package com.gc.hunter.xls.base;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.File;
import java.io.RandomAccessFile;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import jxl.Sheet;
import jxl.Workbook;
import net.sf.json.JSONObject;

import com.gc.hunter.MainFrame;
import com.gc.hunter.MainFrame.ExportType;
import static com.gc.hunter.MainFrame.exportResourcePath;
import com.gc.hunter.ReadSprite;
import com.gc.hunter.tools.ExcelUtil;
import com.gc.hunter.tools.GeneratorUtil;
import com.gc.hunter.tools.IniEditor;
import com.gc.hunter.tools.SqlConnect;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.DateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import jxl.write.WritableCellFeatures;

/**
 * TODO 1.反射优化 添加反射方法的cache 反向工程 遍历所有的Methods,从方法找key的value
 *
 * @author dell
 *
 */
public class ReadBase {

    private ExportType curExportType;
    public static final String ConfigName = "config.ini";
    private static boolean isBase64;
    private static IniEditor setting;
    // 导出clientbin文件
    private String binFiles;
    // 导出DB文件
    private String dbFileName;
    // 导出文件的编码格式，默认UTF-8
    private String FileEncode;
    // key的行号
    private int keyRow = 2;
    // key类型的行号
    private int keyTypeRow_forMysql = 3;
    // key类型的行号
    private int keyTypeRow_forSqlite = 4;
    // xls sheet数组
    private String allSheetNameArray[];
    // key数据
    private Map<String, List<String>> tableKeyMap = new HashMap<String, List<String>>();
    // keyType数据
    private Map<String, List<String>> tableKeyTypeMap_forMysql = new HashMap<String, List<String>>();
    // keyType数据
    private Map<String, List<String>> tableKeyTypeMap_forSqlite = new HashMap<String, List<String>>();
    // keyType Index 数据
    private Map<String, List<Integer>> tableKeyTypeIndexMap_forMysql = new HashMap<String, List<Integer>>();
    // keyType Index 数据
    private Map<String, List<Integer>> tableKeyTypeIndexMap_forSqlite = new HashMap<String, List<Integer>>();
    // 原始的String数据集合
    private Map<String, List<Map<String, String>>> tableDataMap = new HashMap<String, List<Map<String, String>>>();
    // 转成的javabean数据
    private Map<String, List<Object>> tableObjMap = new HashMap<String, List<Object>>();
    // 需要导出的sheet
    private String sheetNameClient[];
    private Map<String, String> tableMap = new HashMap<String, String>();
    private Map<String, String> JavaBeanNameMap = new HashMap<String, String>();
    // model name
    private String model;
    public String JavaBeanName[] ;
    public boolean isExportServer = false;
    public boolean isExportClient = false;
    public boolean isNeedExportBeans = false;
    public boolean isExportErlang = false;
    public boolean isExportText = false;

    public ReadBase() {
    }

    public ReadBase(boolean isUseBase64, String group, ExportType type) throws Exception {

        isBase64 = isUseBase64;
        curExportType = type;

        setting = new IniEditor();
        setting.load(ConfigName);

        binFiles = setting.get(group, "binFiles");

        dbFileName = setting.get(group, "dbName");

        keyRow = Integer.parseInt(setting.get(group, "keyRow"));
        keyTypeRow_forMysql = keyRow + 1;
        keyTypeRow_forSqlite = keyRow + 2;

        FileEncode = setting.get(group, "binFileEncode");
        if (FileEncode == null) {
            FileEncode = "UTF-8";
        }

        model = setting.get(group, "model");

        sheetNameClient = setting.get(group, "sheetClient").split(",");

        if (sheetNameClient != null) {
            String[] tableName = setting.get(group, "table").split(",");
            for (int i = 0; i < sheetNameClient.length; i++) {
                if (!sheetNameClient[i].trim().equals("") && !tableName[i].trim().equals("")) {
                    tableMap.put(sheetNameClient[i].trim(), tableName[i].trim());
                }
            }
        }

        String tempString = setting.get(group, "exportServer");
        isExportServer = tempString != null && tempString.equalsIgnoreCase("true");
        tempString = setting.get(group, "exportClient");
        isExportClient = tempString != null && tempString.equalsIgnoreCase("true");
        tempString = setting.get(group, "javaBean");
        isNeedExportBeans = tempString != null && tempString.equalsIgnoreCase("true");

        if (sheetNameClient != null) {
            String temp = setting.get(group, "JavaBeanName") ;
            if(temp != null) {
                JavaBeanName = temp.split(",");
                for (int i = 0; i < sheetNameClient.length; i++) {
                    if (!sheetNameClient[i].trim().equals("") && !JavaBeanName[i].trim().equals("")) {
                        JavaBeanNameMap.put(sheetNameClient[i].trim(), JavaBeanName[i].trim());
                    }
                }
            }
        }
        
        
        //导出文本
        tempString = setting.get(group, "exportText");
        isExportText = tempString != null && tempString.equalsIgnoreCase("true");

        String XlsFileName = setting.get(group, "sheetFiles");
        workbook = ExcelUtil.getWorkBook(XlsFileName);
       
    }

    public String checkExist() {
        for (String tmp : sheetNameClient) {
            if ( workbook.getSheet(tmp) == null) {
                return tmp;
            }
        }
        return null;
    }

    public void parse() throws Exception {
         parse(workbook);
    }
    
    private Workbook workbook ;
    private Class<?> getClass(String type) throws Exception {
        Class<?> clazz = null;
        if (type.equals("INT")) {
            clazz = Integer.class;
        } else if (type.equals("TEXT")) {
            clazz = String.class;
        }
        return clazz;
    }

    private Object getValue(String type, Sheet curSheet, int col, int row) {
        Object obj = null;
        if (type.equals("INT")) {
            obj = ExcelUtil.getInt(curSheet, col, row);
        } else if (type.equals("TEXT")) {
            obj = ExcelUtil.getString(curSheet, col, row);
        }
        return obj;
    }

    private Class<?> getModelClass(List<String> keyTypeList) throws Exception {
        // 反射
        Class<?> clazz = null;

        try {
            String modelName = "com.gc.hunter.dao.model." + model;
            clazz = Class.forName(modelName);
        } catch (Exception e) {
            String modelName = "com.gc.hunter.dao.model." + model + "WithBLOBs";
            clazz = Class.forName(modelName);
        }
        return clazz;
    }

    /**
     * 把xls解析成对象
     *
     * @param curSheet
     * @param row
     * @return
     * @throws Exception
     */
    private Object encode(Class<?> clazz, Sheet curSheet, List<Integer> keyTypeIndexList, List<String> keyList,
            List<String> keyTypeList, int row) throws Exception {
        Object obj = clazz.newInstance();
        int col = 0;
        for (Integer index : keyTypeIndexList) {
            String method = "set" + getMethodName(keyList.get(index));
            String type = keyTypeList.get(col);
            Class<?> clazzPara = getClass(type);
            Method m = clazz.getMethod(method, clazzPara);
            Object value = getValue(type, curSheet, col, row);
            col++;
            m.invoke(obj, value);
        }
        return obj;
    }

    /**
     * 解析Excel
     *
     * @param workbook
     * @throws Exception
     */
    private void parse(Workbook workbook) throws Exception {

        Sheet[] allSheet = workbook.getSheets();
        allSheetNameArray = new String[allSheet.length];
        for (int s = 0; s < allSheet.length; s++) {
            allSheetNameArray[s] = allSheet[s].getName();
        }
        String stateBuffer = new String();
        
        for (int page = 0; page < allSheet.length; page++) {
            Sheet curSheet = allSheet[page];
            String SheetName = curSheet.getName();


            if (!checkExport(SheetName)) {
                continue;
            }

            // key
            List<String> keyList = new ArrayList<String>();
            // key type
            List<String> keyTypeList = new ArrayList<String>();
            // key type index
            List<Integer> keyTypeIndexList = new ArrayList<Integer>();
            // string data
            List<Map<String, String>> dataList = new ArrayList<Map<String, String>>();
            // obj data
            List<Object> objList = new ArrayList<Object>();

            // 确定页内行数
            int rowMax = 0;
            for (int row = 0; row < curSheet.getRows(); row++) {
                if (curSheet.getCell(0, row).getContents().toString().trim().equals("")
                        && row > keyTypeRow_forSqlite) {
                    break;
                }
                rowMax++;
            }
            
//             //取得当前格子的注释
//            String comment = curSheet.getCell(0, 0).getCellFeatures().getComment() ;
//            System.out.println(" comment = " + comment);
            
            // get key list
            for (int col = 0; col < curSheet.getRow(keyRow).length; col++) {
                String key = ExcelUtil.getString(curSheet, col, keyRow);
                if (key == null || key.equals("")) {
                    continue;
                }
                // 读取关键字
                keyList.add(key);
            }
            tableKeyMap.put(SheetName, keyList);

            // get key type list
            for (int col = 0; col < keyList.size(); col++) {
                String key = ExcelUtil.getString(curSheet, col, keyTypeRow_forMysql);
                if (key == null || key.equals("")) {
                    // 空不导出
                    continue;
                }
                // 读取关键字类型
                keyTypeList.add(key);
                keyTypeIndexList.add(col);
            }
            tableKeyTypeMap_forMysql.put(SheetName, keyTypeList);
            tableKeyTypeIndexMap_forMysql.put(SheetName, keyTypeIndexList);

            // key type
            keyTypeList = new ArrayList<String>();
            // key type index
            keyTypeIndexList = new ArrayList<Integer>();
            // get key type list
            for (int col = 0; col < keyList.size(); col++) {
                String key = ExcelUtil.getString(curSheet, col, keyTypeRow_forSqlite);
                if (key == null || key.equals("")) {
                    // 空不导出
                    continue;
                }
                // 读取关键字类型
                keyTypeList.add(key);
                keyTypeIndexList.add(col);
            }

            int ASpriteNameCol = -1;

           /* String ASpriteName = null;

            ReadSprite spritePaser = null;



            for (int row = 0; row <= keyTypeRow_forSqlite && ASpriteNameCol == -1; row++) {
                for (int col = 0; col < keyList.size(); col++) {
                    String sn = ExcelUtil.getString(curSheet, col, row);
                    if (sn != null && sn.indexOf("Asprite::") >= 0) {
                        String[] sp = sn.split("^Asprite::");
                        if (sp.length > 1) {
                            ASpriteNameCol = col;
                            ASpriteName = sp[1];
                            if (ASpriteName != null) {
                                spritePaser = new ReadSprite();
                                spritePaser.paseSprite(ASpriteName);
                            }
                            break;
                        }
                    }
                }
            }*/




            tableKeyTypeMap_forSqlite.put(SheetName, keyTypeList);
            tableKeyTypeIndexMap_forSqlite.put(SheetName, keyTypeIndexList);

            Class<?> clazz = null;
            if (isJavaBeanType()) {
                checkCreateTable(SheetName);
                clazz = getModelClass(keyTypeList);
            }

            //是否为导出状态表
            boolean isActorState = false;
            if(dbFileName.equals("sl.db"))
                isActorState = true;
            int CurID = 0;
            
            //状态文件的include文件，免去了在手动添加include的麻烦
            ArrayList<String> stateFileIncludeFileBuffer = new ArrayList<String>();
             
            // 扫主表列
            for (int row = keyTypeRow_forSqlite + 1; row < rowMax; row++) {
                if (isJavaBeanType()) {// 导出javabean
                    Object model = encode(clazz, curSheet, keyTypeIndexList, keyList, keyTypeList, row);
                    objList.add(model);
                } else {// 直接导成String数据
                    int col = 0;
                    Map<String, String> data = new HashMap<String, String>();
                    
                    
                    String stateName = null;
                    boolean createCol = false;
                        
                    for (String key : keyList) {
                        String s = ExcelUtil.getString(curSheet, col, row);
                        
                        
                        if(isActorState )
                        {
                            //导出状态表
                            if(key.equals("CreateFiles") && s.equals("YES"))
                            {
                                createCol = true;
                                for(int tmpI = 0 ; tmpI < sheetNameClient.length; tmpI++)
                                    if(SheetName.equals(sheetNameClient[tmpI]))
                                    {
                                        createStateFile("状态转换表_(客户端)", SheetName, stateName, tmpI);
                                        stateFileIncludeFileBuffer.add(stateName);
                                        break;
                                    }
                                 
                            } else if(key.equals("State"))
                            {
                                stateName = s;
                                stateBuffer = stateBuffer + "#define	" + s.trim() + "		"
                                         + CurID + "\n";
                                
                            } else if(key.equals("SID"))
                            {
                                CurID = Integer.valueOf(s);
                            }
                        }
                           
                        
                      
                      if (s.indexOf("Asprite::") >= 0 ) {
                            
                            
                            String spriteName = null;
                            String AnimName = null;
                            
                            String[] sp =  s.split("^Asprite::");
                            if(sp.length > 1)
                            {
                                spriteName = sp[1].substring(0, sp[1].indexOf("::"));
                                AnimName = sp[1].substring(sp[1].indexOf("::") + 2);
                            }
           

                            ReadSprite spritePaser = null;

                            if(spriteName != null)
                            {
                                spritePaser = new ReadSprite();
                                spritePaser.paseSprite(spriteName);
                            }

                            int animIndex = -1;
                            if (spritePaser != null) {
                                if (!s.matches("^[1-9]\\d*$"))//不是整数 开始装换
                                {
                                    animIndex = spritePaser.findIndex(AnimName);
                                    //System.out.println(s +" ==== " + animIndex);

                                    if (animIndex >= 0)//找到了
                                    {
                                        s = String.valueOf(animIndex);
                                    }
                                }
                            }
                            
                            data.put(key, s);
                        } else {
                            data.put(key, ExcelUtil.getString(curSheet, col, row));
                        }

                        col++;
                    }
                    
                    //生成include文件
                   

                    for (Map<String, String> temp : dataList) {
                        if (data.get("ID").equalsIgnoreCase(temp.get("ID"))) {
                            throw new Exception("ID重复啦！～");
                        }
                    }

                    dataList.add(data);
                }
                
                
                
            }
            
            //生成d_hero_state_header.h
            if(isActorState)
            {
                 stateBuffer = stateBuffer + "\n\n\n";
                        
                  for(int tmpI = 0 ; tmpI < sheetNameClient.length; tmpI++)
                  {
                        if(SheetName.equals(sheetNameClient[tmpI]))
                        {
                            createStateIncludeFile("状态转换表_(客户端)", SheetName, stateFileIncludeFileBuffer, tmpI);
                            break;
                        }
                   }
                
                
            }
            

            tableDataMap.put(SheetName, dataList);
            tableObjMap.put(SheetName, objList);

            if (objList.size() > 0) {
                process(objList);
            }
        }
        
        //生成iState
        if(dbFileName.equals("sl.db"))
        {
            String iStateFile = setting.get("状态转换表_(客户端)", "iStateFiles");
            writeIStateFile(iStateFile, stateBuffer);
        }
    }

    public static boolean isNumber(String tp) {
        String temp = tp.toUpperCase();
        if (temp.indexOf("INT") >= 0) {
            return true;
        }
        return false;
    }

    public final void exportErlang(String SheetName) {
        File baseErlf = new File("data_Template.erl");
        if (!baseErlf.exists()) {
            MainFrame.append("----------------data_Template.erl 不存在----------------");
            return;
        }
        File baseErlRecordf = new File("data_record.hrl");
        if (!baseErlRecordf.exists()) { //原始文件不存在不让继续
            System.out.println("----------------data_record.hrl 不存在----------------");
            return;
        }

        File ErlRecordf = null;
        try {
            ErlRecordf = new File("erl\\data_record.hrl");
            if (!ErlRecordf.exists()) //如果不存在就复制一个过去
            {
                copyFile(baseErlRecordf, ErlRecordf);
            }
        } catch (Exception e) {
            MainFrame.append(e.toString());
            return;
        }
        ArrayList<String> ErlRecord_erl = LoadFileToArrayList(ErlRecordf);
        //不管是否是新文件，先全部替换一下在说
        replace(ErlRecord_erl, CMD_NAME, "data_record.hrl");
        replace(ErlRecord_erl, CMD_TIME, toDate());
        replace(ErlRecord_erl, CMD_NOTE, "别修改，自动生成的！");
        int posforUp = find(ErlRecord_erl, "@Created");
        if (posforUp != -1) {
            ErlRecord_erl.remove(posforUp);
            ErlRecord_erl.add(posforUp, "%%% @Created : " + toDate());
        }

        try {
            String table = tableMap.get(SheetName);
            List<String> keyList = tableKeyMap.get(SheetName);
            List<String> keyTypeList = tableKeyTypeMap_forMysql.get(SheetName);
            List<Integer> keyTypeIndexList = tableKeyTypeIndexMap_forMysql.get(SheetName);
            List<Map<String, String>> dataList = tableDataMap.get(SheetName);
            List<Object> objList = tableObjMap.get(SheetName);

            String erlModuleName = "data_" + table;
            File Erlf = new File("erl\\" + erlModuleName + ".erl");
            copyFile(baseErlf, Erlf);
            ArrayList<String> tempList_erl = LoadFileToArrayList(Erlf);
            ArrayList<String> erl_upList = new ArrayList<String>();
            ArrayList<String> erl_upRecordList = new ArrayList<String>();

            replace(tempList_erl, CMD_NAME, erlModuleName);
            replace(tempList_erl, CMD_TIME, toDate());
            replace(tempList_erl, CMD_NOTE, "自动生成");

            String fun = null;
            StringBuffer funret = new StringBuffer();
            int i = 0;
            erl_upRecordList.add("-record(" + erlModuleName.toLowerCase() + ", {");
            StringBuffer funrecord = new StringBuffer();
            for (Integer index : keyTypeIndexList) {
                String key = keyList.get(index);
                funrecord.append(key + ",");
            }
            funrecord.deleteCharAt(funrecord.length() - 1);
            for (Map<String, String> obj : dataList) {
                i = 0;
                funret.setLength(0);
                for (Integer index : keyTypeIndexList) {
                    String key = keyList.get(index);
                    String value = obj.get(key);
                    String type = keyTypeList.get(i);

                    if (key.equalsIgnoreCase("id")) {
                        fun = "get(" + value + ")->";
                    }

                    if (isNumber(type)) {
                        if (value.equals("")) {
                            value = ExcelUtil.DEFAULT_INT + "";
                        }
                        funret.append(key + "=" + value + ",");
                    } else {
                        funret.append(key + "=\"" + value + "\",");
                    }

                    i++;
                }

                funret.deleteCharAt(funret.length() - 1);
                erl_upList.add(fun);
                erl_upList.add("#" + erlModuleName + "{");
                erl_upList.add(funret.toString().toLowerCase());
                erl_upList.add("};");
            }

            erl_upRecordList.add(funrecord.toString().toLowerCase());
            erl_upRecordList.add("}).");
            erl_upRecordList.add(0, "%%" + erlModuleName);
            erl_upRecordList.add("%%" + erlModuleName);

            posforUp = find(ErlRecord_erl, "%%" + erlModuleName);
            if (posforUp != -1) {
                ErlRecord_erl.remove(posforUp);
            }
            int posforDown = find(ErlRecord_erl, "%%" + erlModuleName);
            if (posforUp != -1 && posforDown != -1) {
                ArrayListRemoveItem(ErlRecord_erl, posforUp, posforDown);
            }

            ErlRecord_erl.addAll(erl_upRecordList);

            posforUp = find(tempList_erl, CMD_BODY_UP);
            if (posforUp != -1) {
                tempList_erl.remove(posforUp);
                tempList_erl.addAll(posforUp, erl_upList);
            }
            ArrayListSaveToFile(tempList_erl, Erlf);

            ArrayListSaveToFile(ErlRecord_erl, ErlRecordf);

        } catch (Exception e) {
        }
    }
    public static final String CMD_BODY_UP = "$CMD_BODY_UP$";
    public static final String CMD_NAME = "$CMD_NAME$";
    public static final String CMD_NOTE = "$CMD_NOTE$";
    public static final String CMD_ID = "$CMD_ID$";
    public static final String CMD_TIME = "$TIME$";

    public static String toDate() {
        Date now = new Date(System.currentTimeMillis());
        DateFormat d1 = DateFormat.getDateTimeInstance(); //默认语言（汉语）下的默认风格（MEDIUM风格，比如：2008-6-16 20:54:53）
        String str1 = d1.format(now);
        return str1;
    }

    public final void exportErlangModule() throws Exception {
        MainFrame.append("******************************************");
        for (int i = 0; i < allSheetNameArray.length; i++) {
            String SheetName = allSheetNameArray[i];
            if (!checkExport(SheetName)) {
                continue;
            }

            String table = tableMap.get(SheetName);
            MainFrame.append("开始导出[" + SheetName + "]到本地的表[" + dbFileName + "] -> [" + table + "]");
            try {
                exportErlang(SheetName);
            } catch (Exception e) {
                e.printStackTrace();
            }

            MainFrame.append("开始导出[" + SheetName + "]到本地的表[" + dbFileName + "] -> [" + table + "] 成功...");
        }
    }

    public final void exportDBfile(boolean isNeedExportCpp) throws Exception {
        if (!isExportClient) {
            return;
        }
        
        MainFrame.append("******************************************");
        File file = new File(MainFrame.exportResourcePath + "\\db\\" + dbFileName);
        if(file.exists())
        {
            file.delete();
        }
        
        for (int i = 0; i < allSheetNameArray.length; i++) {
            String SheetName = allSheetNameArray[i];
            if (!checkExport(SheetName)) {
                continue;
            }

            String table = tableMap.get(SheetName);
            MainFrame.append("开始导出[" + SheetName + "]到本地的表[" + dbFileName + "] -> [" + table + "]");
                    
            Class.forName("org.sqlite.JDBC");
            Connection conn = DriverManager.getConnection("jdbc:sqlite:" + MainFrame.exportResourcePath + "\\db\\" + dbFileName);

//                testSQlite3(SheetName, conn);

            insertSQlite3(SheetName, conn);
            conn.close();
            MainFrame.append("开始导出[" + SheetName + "]到本地的表[" + dbFileName + "] -> [" + table + "] 成功...");

        }

        // wxsqlite3 加密
        MainFrame.append( "XP 开始对 [" + dbFileName + "] 加密...");
        Runtime.getRuntime().exec("wxsqlite3PWD.exe" + " " + MainFrame.exportResourcePath + "\\db\\" + dbFileName + " 1234");
        MainFrame.append( "XP 已经对 [" + dbFileName + "] 加密成功...");
        
        if (isNeedExportCpp) {
            MainFrame.append("开始导出C++文件[" + model + "]");
            CreateSQliteCPP_h(sheetNameClient[0]);
            MainFrame.append("开始导出C++文件[" + model + "]  成功...");
        }
        
        if (isExportText) {
            Create_Text_h(sheetNameClient[0]);
            if (model.equalsIgnoreCase("DB_Text"))
            {
                Create_Text_List(sheetNameClient[0]);
            }
        }
    }

    /**
     * 导出客户端数据
     */
    public final void exportClient() throws Exception {


        // 生成的json文件名
        File dbfile = new File(binFiles);
        if (!dbfile.createNewFile()) {
            dbfile.delete();
            dbfile.createNewFile();
        }
        RandomAccessFile raf = new RandomAccessFile(dbfile, "rw");

        Map<String, Object> JSonOutPutMap = new HashMap<String, Object>();
        for (int i = 0; i < allSheetNameArray.length; i++) {
            String SheetName = allSheetNameArray[i];
            if (!checkExport(SheetName)) {
                continue;
            }

            MainFrame.append("******************************************");
            MainFrame.append("开始导出客户端" + SheetName);

            // 组装jsonmap数据格式
            List<Map<String, String>> dataList = tableDataMap.get(SheetName);

            Map<String, Object> curPageMap = new HashMap<String, Object>();

            curPageMap.put("table", dataList);
            JSonOutPutMap.put(SheetName, curPageMap);
            // json
        }

        JSONObject JSonOutPut = JSONObject.fromObject(JSonOutPutMap);
        if (isBase64) {
            String s = new sun.misc.BASE64Encoder().encode(JSonOutPut.toString().getBytes(FileEncode));
            raf.write(s.getBytes());
        } else {
            if (FileEncode.length() > 0) {
                raf.write(JSonOutPut.toString().getBytes(FileEncode));
            } else {
                raf.write(JSonOutPut.toString().getBytes());
            }
        }

        MainFrame.append("" + MainFrame.getNowTime() + " 导出成功...");
        MainFrame.append("******************************************");

        raf.close();
    }

    /**
     * 特殊处理部分
     */
    protected void process(List<Object> objList) {
    }

    /**
     * 限定导出的sheet
     *
     * @return
     */
    private final boolean checkExport(String sheet) {
        for (String tmp : sheetNameClient) {
            if (tmp.equals(sheet)) {
                return true;
            }
        }
        return false;
    }

    public ArrayList<DbField> getExistTableField (ResultSet rs) {
            ArrayList<DbField> ret = new ArrayList<DbField>();
        try {
            rs.first();
            do{
                
                DbField dbField = new DbField(rs);
                ret.add(dbField);
                
            } while(rs.next()) ;
            
            
        } catch (Exception ex) {
            ex.printStackTrace();
            Logger.getLogger(ReadBase.class.getName()).log(Level.SEVERE, null, ex);
        }
            return ret;
    }
    
    public String getOldTableIndex (String table) {
        String temp = null;
        try {
            ResultSet rs = SqlConnect.getInstance().querySQL( "show create table " + table ) ;
            if(rs != null)
            {
                rs.first();
                temp = rs.getString("Create Table") ;
                String list[] = temp.split("\n") ;
                boolean isStartKey = false;
                temp = "";
                for( String n : list )
                {
                    if( n.trim().startsWith( "PRIMARY KEY" )) //PRIMARY KEY表示开始索引
                        isStartKey = true;
                    else if( n.trim().startsWith( ")" ))//创建的表结构，只有最后一行的最开始是右半边括号
                        isStartKey = false;

                    if(isStartKey)
                        temp += n ;

                }
                System.out.println( temp );
            }
        } catch (Exception ex) {
            Logger.getLogger(ReadBase.class.getName()).log(Level.SEVERE, null, ex);        
        }
        return temp;
    }
    
    /**
     * 导出服务器数据
     */
    public final void exportSQL() throws Exception {
        if (!isExportServer) {
            return;
        }

        MainFrame.append("******************************************");
        for (int i = 0; i < allSheetNameArray.length; i++) {
            String SheetName = allSheetNameArray[i];
            if (!checkExport(SheetName)) {
                continue;
            }

            String table = tableMap.get(SheetName);
            MainFrame.append("开始导出[" + SheetName + "]到服务器上的表[" + table + "]");

            
//            ArrayList<DbField> existTable ;
//            ArrayList<DbField> newTable ;
//            ResultSet rs = SqlConnect.getInstance().querySQL( "desc " + table ) ;
//            if(rs != null)
//            {
//                existTable = getExistTableField(rs) ;
//                rs.close();
//            }
//            SqlConnect.getInstance().delete(table + "_temp"); //把临时表删除掉
//            createTable(SheetName);
//            rs = SqlConnect.getInstance().querySQL( "desc " + table + "_temp") ;
//            if(rs != null)
//            {
//                newTable = getExistTableField(rs) ;
//                rs.close();
//            }
//            createAlterTable( SheetName ) ;
            
            String oldTableIndex = null;
            if (SqlConnect.getInstance().isExist(table))
            {
                oldTableIndex = getOldTableIndex(table) ;
            }
            
            SqlConnect.getInstance().delete(table + "_temp"); //把临时表删除掉
            if (!SqlConnect.getInstance().isExist(table + "_temp")) { //先创建一个临时表，最后再改名
                createTable(SheetName , oldTableIndex);
                if (isNeedExportBeans && false)// create dao 用另外的工具导出，这里不用了
                {
                    GeneratorUtil.exec();
                }
            }

            //表已经存在，需要更新操作
            {
                SqlConnect.getInstance().clear(table + "_temp");          // CLEAR
                insertSQL(SheetName);			// insert
            }

            SqlConnect.getInstance().delete(table);
            SqlConnect.getInstance().rename(table + "_temp", table);

            MainFrame.append("导出[" + SheetName + "]到服务器上的表[" + table + "] 成功...");
            String BeanName = JavaBeanNameMap.get(SheetName) ;
            String tempPath = MainFrame.remakeGeneratorConfig( table, BeanName ) ;
            MainFrame.remakeMyBatisDao( tempPath, BeanName );
        }
    }

    private void updateTable(String SheetName) {
        try {
            String table = tableMap.get(SheetName);
            ResultSet rs = getTableDesc(table);
            rs.last();
            int c = rs.getRow();
            rs.first();

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private ResultSet getTableDesc(String TableName) throws Exception {
        return SqlConnect.getInstance().querySQL("desc " + TableName);
    }

    /**
     * 把对象解析成sql命令
     *
     * @param curSheet
     * @param row
     * @return
     * @throws Exception
     */
    private String decode(Class<?> clazz, Object obj,
            List<Integer> keyTypeIndexList,
            List<String> keyList) throws Exception {

        StringBuffer sb = new StringBuffer();

        for (Integer index : keyTypeIndexList) {
            String method = "get" + getMethodName(keyList.get(index));
            Method m = clazz.getMethod(method);
            Object value = m.invoke(obj);
            sb.append("'");
            sb.append(value);
            sb.append("',");
        }

        sb.deleteCharAt(sb.length() - 1);
        return sb.toString();
    }

    public String checkNumberValue (String value) {
        if (value.equals("")) {
            value = ExcelUtil.DEFAULT_INT + "";
        } else if (value.equalsIgnoreCase("true") || value.equalsIgnoreCase("yes")) {
            value = "1";
        }else if (value.equalsIgnoreCase("false") || value.equalsIgnoreCase("no")) {
            value = "0";
        }
        return value;
    }
    
    /**
     * 插入数据
     *
     * @throws Exception
     */
    private void insertSQL(String SheetName) throws Exception {
        String table = tableMap.get(SheetName);

        List<String> keyList = tableKeyMap.get(SheetName);
        List<String> keyTypeList = tableKeyTypeMap_forMysql.get(SheetName);
        List<Integer> keyTypeIndexList = tableKeyTypeIndexMap_forMysql.get(SheetName);
        List<Map<String, String>> dataList = tableDataMap.get(SheetName);
        List<Object> objList = tableObjMap.get(SheetName);

        if (dataList.size() <= 0) {
            return;
        }

        table = table + "_temp";

        StringBuffer sb = new StringBuffer();
        sb.append("INSERT INTO `" + table + "` (");

        for (Integer index : keyTypeIndexList) {
            sb.append("`" + keyList.get(index) + "`,");
        }

        sb.deleteCharAt(sb.length() - 1);
        sb.append(") VALUES ");

        if (isJavaBeanType()) {
            Class<?> clazz = getModelClass(keyTypeList);
            for (Object obj : objList) {
                sb.append("(");
                sb.append(decode(clazz, obj, keyTypeIndexList, keyList));
                sb.append("),");
            }
        } else {
            for (Map<String, String> obj : dataList) {
                sb.append("(");
                int i = 0;
                for (Integer index : keyTypeIndexList) {
                    String key = keyList.get(index);
                    String value = obj.get(key).trim();
                    String type = keyTypeList.get(i);
                    if (isMysqlNumberType(type)) {
                        value = checkNumberValue( value );
                    }
                    if (type.equals("DateTime")) {
                        if (value.equals("")) {
                            sb.append("null,");
                        } else { 
                            sb.append("'");
                            sb.append(value);
                            sb.append("',");
                        }
                    } else {
                        sb.append("'");
                        sb.append(value);
                        sb.append("',");
                    }
                    i++;
                }
                sb.deleteCharAt(sb.length() - 1);
                sb.append("),");
            }
        }

        sb.deleteCharAt(sb.length() - 1);
        sb.append(";");
        SqlConnect.getInstance().execSQL(sb.toString());
    }

    private boolean isMysqlNumberType(String type) {
        return type.equalsIgnoreCase("TINYINT")
                || type.equalsIgnoreCase("SMALLINT")
                || type.equalsIgnoreCase("INT")
                || type.equalsIgnoreCase("BIGINT")
                || type.startsWith("INT")
                || type.startsWith("FLOAT")
                || type.startsWith("DOUBLE")
                ;
    }

    private void CreateSQliteCPP_h(String SheetName) {
        try {
            String table = tableMap.get(SheetName);
            List<String> keyList = tableKeyMap.get(SheetName);
            List<String> keyTypeList = tableKeyTypeMap_forSqlite.get(SheetName);
            List<Integer> keyTypeIndexList = tableKeyTypeIndexMap_forSqlite.get(SheetName);
            List<Map<String, String>> dataList = tableDataMap.get(SheetName);
            List<Object> objList = tableObjMap.get(SheetName);

            ArrayList<String> baseDB_h = new ArrayList<String>();
            ArrayList<String> baseDB_cpp = new ArrayList<String>();

            File base_c_h_f = new File("DB_Template.h");
            if (!base_c_h_f.exists()) {
                System.out.println("----------------DB_Template.h 不存在----------------\n");
                return;
            }
            File base_c_cpp_f = new File("DB_Template.cpp");
            if (!base_c_cpp_f.exists()) {
                System.out.println("----------------DB_Template.cpp 不存在----------------\n");
                return;
            }

            File mubiao_c_h_f = new File(MainFrame.exportCodePath + "\\Database\\base\\" + model + ".h");
            File mubiao_c_cpp_f = new File(MainFrame.exportCodePath + "\\Database\\base\\" + model + ".cpp");

            copyFile(base_c_h_f, mubiao_c_h_f);
            copyFile(base_c_cpp_f, mubiao_c_cpp_f);

            InputStreamReader reader = new InputStreamReader(new FileInputStream(mubiao_c_h_f), "UTF-8");
            BufferedReader input = new BufferedReader(reader);
            String tempString;
            while ((tempString = input.readLine()) != null) {
                baseDB_h.add(tempString);
            }
            input.close();
            reader.close();
            reader = new InputStreamReader(new FileInputStream(mubiao_c_cpp_f), "UTF-8");
            input = new BufferedReader(reader);
            tempString = null;
            while ((tempString = input.readLine()) != null) {
                baseDB_cpp.add(tempString);
            }
            input.close();
            reader.close();

            ArrayList<String> tempForStatic_h = new ArrayList<String>();
            ArrayList<String> tempForBody_h = new ArrayList<String>();
            ArrayList<String> tempForCreate_cpp = new ArrayList<String>();

            replace(baseDB_h, "$CMD_NAME$", model);
            replace(baseDB_cpp, "$CMD_NAME$", model);

            tempForStatic_h.add("string " + model + "::dbsrcFileName = \"db/" + dbFileName + "\";");
            tempForStatic_h.add("string " + model + "::dbdesFileName = \"" + dbFileName + "\";");
            for (int i = 0; i < sheetNameClient.length; i++) {
                String tablename = tableMap.get(sheetNameClient[i]);
                if (tablename != null) {
                    if (i > 0) {
                        tempForStatic_h.add("string " + model + "::dbTableName_" + i + " = \"" + tablename + "\";");
                    } else {
                        tempForStatic_h.add("string " + model + "::dbTableName = \"" + tablename + "\";");
                    }
                }
            }

            int posforUp = find(baseDB_cpp, "$CMD_STATIC$");
            baseDB_cpp.remove(posforUp);
            baseDB_cpp.addAll(posforUp, tempForStatic_h);

            tempForStatic_h.clear();

            tempForStatic_h.add("static string dbsrcFileName ;");
            tempForStatic_h.add("static string dbdesFileName ;");

            for (int i = 0; i < sheetNameClient.length; i++) {
                String tablename = tableMap.get(sheetNameClient[i]);
                if (tablename != null) {
                    if (i > 0) {
                        tempForStatic_h.add("static string dbTableName_" + i + " ;");
                    } else {
                        tempForStatic_h.add("static string dbTableName ;");
                    }
                }
            }

            int count = 0;
            for (Integer index : keyTypeIndexList) {
                String key = keyList.get(index);
                String type = keyTypeList.get(count);
                if (type.equals("INT")) {
                    tempForBody_h.add("int " + key + " ;");
                    tempForCreate_cpp.add(key + " = atoi(column_value[" + model + "_" + key + "]);");
                } else if (type.equals("REAL")) {
                    tempForBody_h.add("float " + key + " ;");
                    tempForCreate_cpp.add(key + " = atof(column_value[" + model + "_" + key + "]);");
                } else {
                    tempForBody_h.add("string " + key + " ;");
                    tempForCreate_cpp.add(key + " = string(column_value[" + model + "_" + key + "]);");
                }
                tempForStatic_h.add("static const int " + model + "_" + key + " = " + count + " ;");
                count++;
            }

            posforUp = find(baseDB_h, "$CMD_STATIC$");
            baseDB_h.remove(posforUp);
            baseDB_h.addAll(posforUp, tempForStatic_h);

            posforUp = find(baseDB_h, "$CMD_BODY$");
            baseDB_h.remove(posforUp);
            baseDB_h.addAll(posforUp, tempForBody_h);

            posforUp = find(baseDB_cpp, "$CMD_CREATE$");
            baseDB_cpp.remove(posforUp);
            baseDB_cpp.addAll(posforUp, tempForCreate_cpp);

            FormatCode(baseDB_h);
            FormatCode(baseDB_cpp);

            StringBuffer baseTemp = new StringBuffer();
            for (int i = 0; i < baseDB_h.size(); i++) {
                baseTemp.append(baseDB_h.get(i));
                if (!baseDB_h.get(i).endsWith("\n")) {
                    baseTemp.append(" \n");
                }
            }
            OutputStreamWriter write = new OutputStreamWriter(new FileOutputStream(mubiao_c_h_f), "UTF-8");
            BufferedWriter output = new BufferedWriter(write);
            output.write(baseTemp.toString());
            output.flush();
            output.close();
            write.close();

            baseTemp.setLength(0);
            for (int i = 0; i < baseDB_cpp.size(); i++) {
                baseTemp.append(baseDB_cpp.get(i));
                if (!baseDB_cpp.get(i).endsWith("\n")) {
                    baseTemp.append(" \n");
                }
            }

            write = new OutputStreamWriter(new FileOutputStream(mubiao_c_cpp_f), "UTF-8");
            output = new BufferedWriter(write);
            output.write(baseTemp.toString());
            output.flush();
            output.close();
            write.close();



            System.out.println("");
        } catch (Exception ex) {
            Logger.getLogger(ReadBase.class.getName()).log(Level.SEVERE, null, ex);
            ex.printStackTrace();
        }
    }

    private void Create_Text_h(String SheetName) {
        try {
            String table = tableMap.get(SheetName);
            List<String> keyList = tableKeyMap.get(SheetName);
            List<String> keyTypeList = tableKeyTypeMap_forSqlite.get(SheetName);
            List<Integer> keyTypeIndexList = tableKeyTypeIndexMap_forSqlite.get(SheetName);
            List<Map<String, String>> dataList = tableDataMap.get(SheetName);
            List<Object> objList = tableObjMap.get(SheetName);

            ArrayList<String> baseDB_h = new ArrayList<String>();
//            ArrayList<String> baseDB_cpp = new ArrayList<String>();

            File base_c_h_f = new File("TEXT_Template.h");
            if (!base_c_h_f.exists()) {
                System.out.println("----------------DB_Template.h 不存在----------------\n");
                return;
            }
            
            String name_h = model + "_const.h";
            File mubiao_c_h_f = new File(MainFrame.exportCodePath + "\\Database\\base\\" + name_h);

            copyFile(base_c_h_f, mubiao_c_h_f);

            InputStreamReader reader = new InputStreamReader(new FileInputStream(mubiao_c_h_f), "UTF-8");
            BufferedReader input = new BufferedReader(reader);
            String tempString;
            while ((tempString = input.readLine()) != null) {
                baseDB_h.add(tempString);
            }
            input.close();
            reader.close();

            ArrayList<String> tempForStatic_h = new ArrayList<String>();
//            ArrayList<String> tempForBody_h = new ArrayList<String>();
//            ArrayList<String> tempForCreate_cpp = new ArrayList<String>();
            
            String tmp = model.toUpperCase() + "_CONST"; 
            replace(baseDB_h, "$CMD_NAME$", tmp);
//            replace(baseDB_cpp, "$CMD_NAME$", model);

//            tempForStatic_h.add("string " + model + "::dbsrcFileName = \"db/" + dbFileName + "\";");
//            tempForStatic_h.add("string " + model + "::dbdesFileName = \"" + dbFileName + "\";");
//            for (int i = 0; i < sheetNameClient.length; i++) {
//                String tablename = tableMap.get(sheetNameClient[i]);
//                if(tablename != null)
//                {
//                    if(i > 0)
//                        tempForStatic_h.add("string " + model + "::dbTableName_"+i+" = \"" + tablename + "\";");
//                    else
//                        tempForStatic_h.add("string " + model + "::dbTableName = \"" + tablename + "\";");
//                }
//            }

//            int posforUp = find(baseDB_cpp, "$CMD_STATIC$");
//            baseDB_cpp.remove(posforUp);
//            baseDB_cpp.addAll(posforUp, tempForStatic_h);

            tempForStatic_h.clear();

//            tempForStatic_h.add("static string dbsrcFileName ;");
//            tempForStatic_h.add("static string dbdesFileName ;");

//            for (int i = 0; i < sheetNameClient.length; i++) {
//                String tablename = tableMap.get(sheetNameClient[i]);
//                if(tablename != null)
//                {
//                    if(i > 0)
//                        tempForStatic_h.add("static string dbTableName_"+i+" ;");
//                    else
//                        tempForStatic_h.add("static string dbTableName ;");
//                }
//            }

            int count = 0;
//            for (Integer index : keyTypeIndexList) {
//                String key = keyList.get(index);
//                String type = keyTypeList.get(count);
//                tempForStatic_h.add("static const int " + model + "_" + key + " = " + count + " ;");
//                count++;
//            }

            for (Map<String, String> obj : dataList) {
                if (model.equalsIgnoreCase("DB_Text"))
                {
                    String id = obj.get("ID");
                    String key = obj.get("skey");
                    tempForStatic_h.add("static const int TEXT_" + key.toUpperCase() + "         = " + id + "; ");
                }
                else if (model.equalsIgnoreCase("XTipMsg"))
                {
                    String id = obj.get("ID");
                    String key = obj.get("skey");
                    tempForStatic_h.add("static const int TIP_" + key.toUpperCase() + "         = " + id + "; ");
                }
                count++;
            }
            MainFrame.append("导出 " + name_h + " count: " + count);
            int posforUp = find(baseDB_h, "$CMD_STATIC$");
            baseDB_h.remove(posforUp);
            baseDB_h.addAll(posforUp, tempForStatic_h);

            FormatCode(baseDB_h);

            StringBuilder baseTemp = new StringBuilder();
            for (int i = 0; i < baseDB_h.size(); i++) {
                baseTemp.append(baseDB_h.get(i));
                if (!baseDB_h.get(i).endsWith("\n")) {
                    baseTemp.append(" \n");
                }
            }
            OutputStreamWriter write = new OutputStreamWriter(new FileOutputStream(mubiao_c_h_f), "UTF-8");
            BufferedWriter output = new BufferedWriter(write);
            output.write(baseTemp.toString());
            output.flush();
            output.close();
            write.close();

            System.out.println("");
        } catch (Exception ex) {
            Logger.getLogger(ReadBase.class.getName()).log(Level.SEVERE, null, ex);
            ex.printStackTrace();
        }
    }

    private void Create_Text_List(String SheetName) {
        try {
            String table = tableMap.get(SheetName);
            List<String> keyList = tableKeyMap.get(SheetName);
            List<String> keyTypeList = tableKeyTypeMap_forSqlite.get(SheetName);
            List<Integer> keyTypeIndexList = tableKeyTypeIndexMap_forSqlite.get(SheetName);
            List<Map<String, String>> dataList = tableDataMap.get(SheetName);
            List<Object> objList = tableObjMap.get(SheetName);

            ArrayList<String> baseDB_h = new ArrayList<String>();
            File mubiao_c_h_f = new File(MainFrame.exportCodePath + "\\..\\..\\_master_data\\gfx\\tileset\\level\\TEXT.list");

            if (!mubiao_c_h_f.exists()) {
                mubiao_c_h_f.createNewFile();
            } else {
                mubiao_c_h_f.delete();
                mubiao_c_h_f.createNewFile();
            }

            InputStreamReader reader = new InputStreamReader(new FileInputStream(mubiao_c_h_f), "UTF-8");
            BufferedReader input = new BufferedReader(reader);
            String tempString;
            while ((tempString = input.readLine()) != null) {
                baseDB_h.add(tempString);
            }
            input.close();
            reader.close();

            ArrayList<String> tempForStatic_h = new ArrayList<String>();

            tempForStatic_h.clear();

            int count = 0;
            for (Map<String, String> obj : dataList) {
                String id = obj.get("ID");
                String key = obj.get("skey");
                tempForStatic_h.add(id + "=" + "\"" + key.toUpperCase() + "_TEXT" + "\"");
                count++;
            }
            MainFrame.append("导出 TEXT.list count: " + count);

            baseDB_h.addAll(0, tempForStatic_h);

            FormatCode(baseDB_h);

            StringBuilder baseTemp = new StringBuilder();
            for (int i = 0; i < baseDB_h.size(); i++) {
                baseTemp.append(baseDB_h.get(i));
                if (!baseDB_h.get(i).endsWith("\n")) {
                    baseTemp.append(" \n");
                }
            }
            OutputStreamWriter write = new OutputStreamWriter(new FileOutputStream(mubiao_c_h_f), "UTF-8");
            BufferedWriter output = new BufferedWriter(write);
            output.write(baseTemp.toString());
            output.flush();
            output.close();
            write.close();

            System.out.println("");
        } catch (Exception ex) {
            Logger.getLogger(ReadBase.class.getName()).log(Level.SEVERE, null, ex);
            ex.printStackTrace();
        }
    }

    private void testSQlite3(String SheetName, Connection conn) {
        try {
            Statement stat = conn.createStatement();
            conn.setAutoCommit(true);
            String table = tableMap.get(SheetName);
            StringBuffer sb = new StringBuffer();
            ResultSet rs = stat.executeQuery("select name from sqlite_master where type='table' order by name;");
            System.out.println("-----------------------------");
            while (rs.next()) {
                System.out.println(" name = " + rs.getString("name"));
            }
            System.out.println("-----------------------------");
            rs.close();

            rs = stat.executeQuery("PRAGMA table_info([" + table + "])");
            System.out.println("-----------------------------");

            while (rs.next()) {
                sb.setLength(0);
                for (int i = 1; i <= 6; i++) {
                    sb.append(rs.getString(i));
                    sb.append(",");
                }
                System.out.println(sb.toString());
            }
            System.out.println("-----------------------------");
            rs.close();

            stat.execute("ALTER TABLE " + table + " ADD COLUMN citycol ;");

            rs = stat.executeQuery("PRAGMA table_info([" + table + "])");
            System.out.println("-----------------------------");

            while (rs.next()) {
                sb.setLength(0);
                for (int i = 1; i <= 6; i++) {
                    sb.append(rs.getString(i));
                    sb.append(",");
                }
                System.out.println(sb.toString());
            }
            System.out.println("-----------------------------");
            rs.close();

//            sb.setLength(0);
//            sb.append("insert into " + table + " values (");
//            for (Integer index : keyTypeIndexList) {
//                sb.append("?,");
//            }
//            sb.deleteCharAt(sb.length() - 1);
//            sb.append(");");
//
//            PreparedStatement prep = conn.prepareStatement(sb.toString());
//            prep.setInt(PrepIndex, Integer.parseInt(value));
//            prep.setString(PrepIndex, value);
//            prep.addBatch();
//
//            conn.setAutoCommit(false);
//            prep.executeBatch();
//            conn.setAutoCommit(true);

        } catch (Exception ex) {
            Logger.getLogger(ReadBase.class.getName()).log(Level.SEVERE, null, ex);
            ex.printStackTrace();
        }
    }

    /**
     * 插入数据
     *
     * @throws Exception
     */
    private void insertSQlite3(String SheetName, Connection conn) throws Exception {
        Statement stat = conn.createStatement();
        String table = tableMap.get(SheetName);

        List<String> keyList = tableKeyMap.get(SheetName);
        List<String> keyTypeList = tableKeyTypeMap_forSqlite.get(SheetName);
        List<Integer> keyTypeIndexList = tableKeyTypeIndexMap_forSqlite.get(SheetName);
        List<Map<String, String>> dataList = tableDataMap.get(SheetName);
        List<Object> objList = tableObjMap.get(SheetName);

        StringBuilder sb = new StringBuilder();
        try {
            stat.executeUpdate("DROP TABLE " + table);
        } catch (SQLException sQLException) {
        }

        int count = 0;
        sb.append("create table " + table + " (");
        for (Integer index : keyTypeIndexList) {
            if (keyTypeList.get(count).equalsIgnoreCase("REAL")) {
                sb.append(keyList.get(index) + " REAL,");
            } else if (keyTypeList.get(count).equalsIgnoreCase("INT")) {
                sb.append(keyList.get(index) + " INTEGER,");
            } else {
                sb.append(keyList.get(index) + " TEXT,");
            }

            count++;
        }
        sb.deleteCharAt(sb.length() - 1);
        sb.append(");");
        stat.executeUpdate(sb.toString());

        System.out.println(" create = " + sb.toString());

        {
            sb.setLength(0);
            sb.append("insert into " + table + " values (");
            for (Integer index : keyTypeIndexList) {
                sb.append("?,");
            }
            sb.deleteCharAt(sb.length() - 1);
            sb.append(");");

            PreparedStatement prep = conn.prepareStatement(sb.toString());
            for (Map<String, String> obj : dataList) {
                int i = 0;
                int PrepIndex = 1;
                for (Integer index : keyTypeIndexList) {
                    String key = keyList.get(index);
                    String value = obj.get(key);
                    String type = keyTypeList.get(i);
                    
                    if (isMysqlNumberType(type)) {
                        value = checkNumberValue( value );
                        prep.setInt(PrepIndex, Integer.parseInt(value));
                    } else if (type.equalsIgnoreCase("REAL")) {
                        value = checkNumberValue( value );
                        prep.setFloat(PrepIndex, Float.parseFloat(value));
                    } else {
                        prep.setString(PrepIndex, value);
                    }
                    PrepIndex++;
                    i++;
                }
                prep.addBatch();
            }

            conn.setAutoCommit(false);
            prep.executeBatch();
            conn.setAutoCommit(true);
        }
    }

    private void checkCreateTable(String SheetName) throws Exception {
        String table = tableMap.get(SheetName);

        if (!SqlConnect.getInstance().isExist(table)) {

            MainFrame.append("******************************************");
            MainFrame.append("创建SQL表" + SheetName);

            createTable(SheetName , null);
            // create dao
            GeneratorUtil.exec();
        }
    }
    
    /**
     * 创建表
     *
     * @throws Exception
     */
    private void createTable(String SheetName ,String oldTableIndex) throws Exception {
        String table = tableMap.get(SheetName);

        List<String> keyList = tableKeyMap.get(SheetName);
        List<String> keyTypeList = tableKeyTypeMap_forMysql.get(SheetName);
        List<Integer> keyTypeIndexList = tableKeyTypeIndexMap_forMysql.get(SheetName);

        StringBuffer sb = new StringBuffer();
        sb.append("CREATE TABLE " + table + "_temp (");
        int i = 0;
        for (Integer index : keyTypeIndexList) {
            String type = keyTypeList.get(i);
            sb.append("`").append(keyList.get(index)).append("` ").append(keyTypeList.get(i)) ;
            if (isMysqlNumberType(type) && type.indexOf("AUTO_INCREMENT") < 0) {
                sb.append(" DEFAULT ").append(ExcelUtil.DEFAULT_INT);
            }
            sb.append(",");
            i++;
        }

        // primary key
        if(oldTableIndex == null)
            sb.append("PRIMARY KEY (" + keyList.get(0) + "));");
        else
            sb.append(oldTableIndex).append(")") ;

        System.out.println(sb.toString()) ;
        

        SqlConnect.getInstance().execSQL(sb.toString());
    }

    /**
     * 方法名
     *
     * @param key
     * @return
     */
    private String getMethodName(String key) {
        String method = key.substring(0, 1).toUpperCase()
                + key.substring(1).toLowerCase();

        int index = method.indexOf("_");
        while (index >= 0) {
            method = method.substring(0, index)
                    + method.substring(index + 1, index + 2).toUpperCase()
                    + method.substring(index + 2);
            index = method.indexOf("_");
        }

        return method;
    }

    /**
     * 是否有BLOG类
     *
     * @return
     */
    private final boolean hasBLOB(List<String> keyTypeList) {
        if (keyTypeList.contains("TEXT")) {
            return true;
        }
        return false;
    }

    /**
     * 受否导出Clinet
     *
     * @return
     */
    private final boolean isJavaBeanType() {
        return curExportType == ExportType.TYPE_SERVER && false;
    }

    // 复制文件
    public static void copyFile(File sourceFile, File targetFile) throws IOException {
        BufferedInputStream inBuff = null;
        BufferedOutputStream outBuff = null;
        try {
            // 新建文件输入流并对它进行缓冲
            inBuff = new BufferedInputStream(new FileInputStream(sourceFile));

            // 新建文件输出流并对它进行缓冲
            outBuff = new BufferedOutputStream(new FileOutputStream(targetFile));

            // 缓冲数组
            byte[] b = new byte[1024 * 5];
            int len;
            while ((len = inBuff.read(b)) != -1) {
                outBuff.write(b, 0, len);
            }
            // 刷新此缓冲的输出流
            outBuff.flush();
        } finally {
            // 关闭流
            if (inBuff != null) {
                inBuff.close();
            }
            if (outBuff != null) {
                outBuff.close();
            }
        }
    }

    public static void replace(ArrayList<String> source, String from, String to) {
        if (from == null || to == null) {
            return;
        }

        String temp = null;
        for (int i = 0; i < source.size(); i++) {
            temp = source.get(i);
            temp = replace(temp, from, to);
            source.set(i, temp);
        }
    }

    public static int find(ArrayList<String> source, String from) {
        int ret = -1;
        String temp = null;
        for (int i = 0; i < source.size(); i++) {
            temp = source.get(i);
            if (temp != null && temp.indexOf(from) >= 0) {
                ret = i;
                break;
            }
        }

        return ret;
    }

    /**
     * 替换字符串
     *
     * @param from String 原始字符串
     * @param to String 目标字符串
     * @param source String 母字符串
     * @return String 替换后的字符串
     */
    public static String replace(String source, String from, String to) {
        if (source == null || from == null || to == null) {
            return null;
        }
        StringBuffer bf = new StringBuffer("");
        int index = -1;
        while ((index = source.indexOf(from)) != -1) {
            bf.append(source.substring(0, index) + to);
            source = source.substring(index + from.length());
            index = source.indexOf(from);
        }
        bf.append(source);
        return bf.toString();
    }

    public String getTab(int tab) {
        String ret = "";
        for (int i = 0; i < tab; i++) {
            ret += '\t';
        }
        return ret;
    }

    public ArrayList<String> LoadFileToArrayList(File f) {
        ArrayList<String> ret = new ArrayList<String>();
        try {
            InputStreamReader reader = new InputStreamReader(new FileInputStream(f), "UTF-8");
            BufferedReader input = new BufferedReader(reader);
            String tempString;
            while ((tempString = input.readLine()) != null) {
                ret.add(tempString);
            }
            input.close();
            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ret;
    }

    public void ArrayListRemoveItem(ArrayList<String> needSave, int low, int up) {
        try {
            int count = up - low;
            for (int i = 0; i <= count; i++) {
                needSave.remove(low);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void ArrayListSaveToFile(ArrayList<String> needSave, File f) {
        try {
            FormatCode(needSave);
            StringBuffer baseTemp = new StringBuffer();
            for (int i = 0; i < needSave.size(); i++) {
                baseTemp.append(needSave.get(i));
                if (!needSave.get(i).endsWith("\n")) {
                    baseTemp.append(" \n");
                }
            }
            OutputStreamWriter write = new OutputStreamWriter(new FileOutputStream(f), "UTF-8");
            BufferedWriter output = new BufferedWriter(write);
            output.write(baseTemp.toString());
            output.flush();
            output.close();
            write.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void FormatCode(ArrayList<String> tempList) {
        int TabNum = 0;
        int ColonTabNum = 0;
        for (int i = 0; i < tempList.size(); i++) {
            String tempString = tempList.get(i).trim();
            //tempString = replace(tempString, "\t", "");
            if (tempString.indexOf("{") >= 0) {//如果是{结尾，就缩进一个tab
                if (tempString.indexOf("}") >= 0) {
                    continue;
                }
                tempString = getTab(TabNum) + tempString;
                tempList.set(i, tempString);
                TabNum++;
            } else if (tempString.indexOf("}") >= 0) {//如果是}结尾，就伸出一个tab
                TabNum--;
                ColonTabNum = 0;
                tempString = getTab(TabNum) + tempString;
                tempList.set(i, tempString);
            } else {
                if (tempString.endsWith(":")) {
                    ColonTabNum = 1;
                    tempString = getTab(TabNum) + tempString;
                } else {
                    tempString = getTab(TabNum + ColonTabNum) + tempString;
                }
                tempList.set(i, tempString);
            }
        }
    }
    
    
    //创建State文件
    static void createStateFile(String group, String sheetName, String stateName, int index) throws IOException {

        
        
        String[] StateFolder = setting.get(group, "StateFolder").split(",");
        //String SheetNameUpperCase = sheetName.toUpperCase();
        String STATE_FILE_ROOT = setting.get(group, "StateFilesRoot");
        String stateHFileName = "" + STATE_FILE_ROOT + StateFolder[index].trim() + "/STATE_LIST/" + stateName + ".h";
        String stateHPPFileName = "" + STATE_FILE_ROOT + StateFolder[index].trim() + "/STATE_LIST/" + stateName + ".hpp";
        File fileH = new File(stateHFileName);
        File fileHPP = new File(stateHPPFileName);
        File parent = fileH.getParentFile();
        if (parent != null && !parent.exists()) {
            parent.mkdirs();
        }
        //生成.h文件
        if (fileH.createNewFile()) {

            String stringBuffer = "DECLARE_STATE_CODE( " + stateName + " )";

            FileWriter stateFWTR = new FileWriter(fileH);
            BufferedWriter stateBFWTR = new BufferedWriter(stateFWTR);

            stateBFWTR.write(stringBuffer);

            if (stateBFWTR != null) {
                stateBFWTR.close();
            }
            if (stateFWTR != null) {
                stateFWTR.close();
            }

            stringBuffer = null;
        }
        //生成.hpp文件
        if (fileHPP.createNewFile()) {

            String stringBuffer = "START_STATE_CODE( " + stateName + " )\n{\n";

            stringBuffer = stringBuffer + "	START_MESSAGE_PROCESS( MESSAGE_ENTER )\n	{\n	}\n ";
            stringBuffer = stringBuffer + "	START_MESSAGE_PROCESS( MESSAGE_UPDATE )\n	{\n	}\n ";
            stringBuffer = stringBuffer + "	START_MESSAGE_PROCESS( MESSAGE_PAINT )\n	{\n	}\n ";
            stringBuffer = stringBuffer + "	START_MESSAGE_PROCESS( MESSAGE_EXIT )\n	{\n	}\n }";

            FileWriter stateFWTR = new FileWriter(fileHPP);
            BufferedWriter stateBFWTR = new BufferedWriter(stateFWTR);

            stateBFWTR.write(stringBuffer);

            if (stateBFWTR != null) {
                stateBFWTR.close();
            }
            if (stateFWTR != null) {
                stateFWTR.close();
            }

            stringBuffer = null;
        }
    }
    //创建状态的include 文件，免去手动添加的麻烦

    static void createStateIncludeFile(String Group, String sheetName, ArrayList<String> stateList, int index) throws IOException {
       

        if (stateList.size() == 0) {
            return;
        }
        
        String[] StateFolder = setting.get(Group, "StateFolder").split(",");
        String[] StateTable = setting.get(Group, "table").split(",");
        String STATE_FILE_ROOT = setting.get(Group, "StateFilesRoot");
        String SheetNameUpperCase = sheetName.toUpperCase();
        
        String stateHFileName = "" + STATE_FILE_ROOT + StateFolder[index].trim() + "/" + StateTable[index] + "Header.h";
        String stateHPPFileName = "" + STATE_FILE_ROOT + StateFolder[index].trim() + "/" + StateTable[index] + "Header.hpp";

        File fileH = new File(stateHFileName);
        File fileHPP = new File(stateHPPFileName);
        File parent = fileH.getParentFile();
        if (parent != null && !parent.exists()) {
            parent.mkdirs();
        }

        if (!fileH.createNewFile()) {
            fileH.delete();
            fileH.createNewFile();
        }

        if (!fileHPP.createNewFile()) {
            fileHPP.delete();
            fileHPP.createNewFile();
        }

        //生成.h文件
        String stringBuffer = "";
        for (int i = 0; i < stateList.size(); i++) {
            stringBuffer = stringBuffer + "#include \"STATE_LIST/" + stateList.get(i) + ".h\"\n";
        }

        FileWriter stateFWTR = new FileWriter(fileH);
        BufferedWriter stateBFWTR = new BufferedWriter(stateFWTR);

        stateBFWTR.write(stringBuffer);

        if (stateBFWTR != null) {
            stateBFWTR.close();
        }
        if (stateFWTR != null) {
            stateFWTR.close();
        }

        stringBuffer = null;

        //生成.hpp文件
        stringBuffer = "";
        for (int i = 0; i < stateList.size(); i++) {
            stringBuffer = stringBuffer + "#include \"STATE_LIST/" + stateList.get(i)
                    + ".hpp\"\n";
        }

        stateFWTR = new FileWriter(fileHPP);
        stateBFWTR = new BufferedWriter(stateFWTR);

        stateBFWTR.write(stringBuffer);

        if (stateBFWTR != null) {
            stateBFWTR.close();
        }
        if (stateFWTR != null) {
            stateFWTR.close();
        }

        stringBuffer = null;

    }
    
     //写iState文件
    static void writeIStateFile(String iStateName, String stateBuffer) throws IOException {
        File stateFile = null;
        FileWriter stateFWTR = null;
        BufferedWriter stateBFWTR = null;


        stateFile = new File(iStateName);
        if (!stateFile.createNewFile()) {
            stateFile.delete();
            stateFile.createNewFile();
        }
        stateFWTR = new FileWriter(stateFile);
        stateBFWTR = new BufferedWriter(stateFWTR);

        stateBFWTR.write(stateBuffer);

        if (stateBFWTR != null) {
            stateBFWTR.close();
        }
        if (stateFWTR != null) {
            stateFWTR.close();
        }

        stateBuffer = null;
    }
}
