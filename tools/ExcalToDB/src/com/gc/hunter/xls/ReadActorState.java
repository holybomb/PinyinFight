package com.gc.hunter.xls;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gc.hunter.MainFrame;
import com.gc.hunter.ReadSprite;
import com.gc.hunter.tools.IniEditor;

import jxl.Sheet;
import jxl.Workbook;
import net.sf.json.JSONObject;

public class ReadActorState {

    public static String sheetNameArray[];
    public static final String ConfigName = "config.ini";
    public static boolean isBase64;
    //	JSon 导出结构
    //	{ 														//JSonOutPutMap
    //		{ page1, 											//curPageMap
    //			{属性(property): {属性1，属性2，属性3...}}			//pagePropertyMap
    //			{状态表(state_table):							
    //				{状态1：{状态属性1，状态属性2...}}				//stateList		
    //				{状态2：{状态属性1，状态属性2...}}
    //				...
    //			}
    //		}
    //		{ page2, 
    //			{属性(property): {属性1，属性2，属性3...}}
    //			{状态表(state_table):
    //				{状态1：{状态属性1，状态属性2...}}
    //				{状态2：{状态属性1，状态属性2...}}
    //				...
    //			}	
    //		}
    //	}
    static IniEditor users;

    public ReadActorState(boolean isUseBase64) throws Exception {
        isBase64 = isUseBase64;
        users = new IniEditor();
        users.load(ConfigName);
        String XlsFileName = users.get("actorState", "sheetFiles");


        Workbook workbook = null;
        try {
            workbook = Workbook.getWorkbook(new File(XlsFileName));
        } catch (Exception e) {
            throw new Exception(XlsFileName + " file to import not found!");
        }

        parse(workbook);

    }

    public void parse(Workbook workbook) throws Exception {
        Sheet sheet = null;
        Sheet[] allSheet = workbook.getSheets();
        sheetNameArray = new String[allSheet.length];
        for (int s = 0; s < allSheet.length; s++) {
            sheetNameArray[s] = allSheet[s].getName();
        }

        String binFileName = users.get("actorState", "binFiles");
        //生成的json文件名
        File dbfile = new File(binFileName);
        if (!dbfile.createNewFile()) {
            dbfile.delete();
            dbfile.createNewFile();
        }

        JSONObject JSonOutPut;
        RandomAccessFile raf = new RandomAccessFile(dbfile, "rw");

        //生成的iState状态文件
        //File stateFile = null;
        //FileWriter stateFWTR = null;
        //BufferedWriter stateBFWTR = null;
        String stateBuffer = new String();
        Map<String, Object> JSonOutPutMap = new HashMap<String, Object>();

        boolean exportOK = true;

        for (int page = 0; page < allSheet.length; page++) {
            //写iState之用
            //表中为State关键字的列号
            int StateCol = -1;
            //表中ID的列号
            int IDCol = -1;
            //生成iState的内容buffer


            //是否用创建状态文件(e.g:STATE_RUN...)的列
            int CreateFilesCol = -1;
            //状态文件的include文件，免去了在手动添加include的麻烦
            ArrayList<String> stateFileIncludeFileBuffer = new ArrayList<String>();

            //清除掉DescriptionForGD列，减小导出数据
            int DescriptionForGD = -1;
            
            //动画的animation 名字
            int AnimationKeyCol = -1;

            Sheet curSheet = allSheet[page];
            String SheetName = curSheet.getName();
            
            //忽略空页和help页
            if (curSheet == null || curSheet.getName().equals("帮助") || curSheet.getName().equals("common_define")) {
                continue;
            }

            Map<String, Object> curPageMap = new HashMap<String, Object>();
            //Map<String, Object> 
            List<Object> stateList = new ArrayList<Object>();

            //获得属性
            //Map<String, Object> pagePropertyMap = getPageProperty(curSheet);
            //插入页属性
            //curPageMap.put("property", pagePropertyMap);


            //确定页内行数
            int rowMax = 0;
            try {
                for (int row = 0; row < curSheet.getRows(); row++) {
                    if (curSheet.getCell(0, row).getContents().toString().trim().equals("")) {
                        break;
                    }
                    rowMax++;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }


            Map<String, String> indexTable = new HashMap<String, String>();//表的列属性映射表，<col, KeyWord>

            for (int col = 0; col < curSheet.getRow(1).length; col++) {
                if (curSheet.getCell(col, 1).getContents().toString().trim().equals("")) {
                    continue;
                }
                //读取属性行（第一行）作为关键字
                indexTable.put("" + col, curSheet.getCell(col, 1).getContents().toString());

                //记录state用于iState
                if (curSheet.getCell(col, 1).getContents().toString().equals("State")) {
                    StateCol = col;
                    stateBuffer = stateBuffer + "// " + curSheet.getName() + " state \n";
                }
                //记录ID用于iState
                if (curSheet.getCell(col, 1).getContents().toString().equals("SID")) {
                    IDCol = col;
                }
                if (curSheet.getCell(col, 1).getContents().toString().equals("CreateFiles")) {
                    CreateFilesCol = col;
                }

                if (curSheet.getCell(col, 1).getContents().toString().equals("DescriptionForGD")) {
                    DescriptionForGD = col;
                }
                
                if (curSheet.getCell(col, 1).getContents().toString().equals("AnimationKeyWord")) {
                    AnimationKeyCol = col;
                }
            }
            



            //扫主表行
            for (int row = 4; row < rowMax; row++) {
                Map<String, String> stateDetailMap = new HashMap<String, String>();
                //扫主表列
                for (int col = 0; col < curSheet.getColumns(); col++) {
                    //检测列是否有属性
                    if (indexTable.get("" + col) == null || indexTable.get("" + col).trim().equals("")) {
                        continue;
                    }
                    // 保持中文字符可被写入到json
                    //System.out.println("curSheet.getCell(col, row).getContents().toString().trim() is " + curSheet.getCell(col, row).getContents().toString().trim());
                    String strUTF8 = curSheet.getCell(col, row).getContents().toString().trim();//URLEncoder.encode(curSheet.getCell(col, row).getContents().toString().trim(), "UTF-8");
                    
                    int animIndex = -1;
                    
                    String spriteName = null;
                    String AnimName = null;

                    if(strUTF8.indexOf("Asprite::") >= 0 )
                    {
                        String[] sp = strUTF8.split("^Asprite::");
                        if(sp.length > 1)
                        {
                            spriteName = sp[1].substring(0, sp[1].indexOf("::"));
                            AnimName = sp[1].substring(sp[1].indexOf("::") + 2);
                        }
                    }
           

                    ReadSprite spritePaser = null;

                    if(spriteName != null)
                    {
                        spritePaser = new ReadSprite();
                        spritePaser.paseSprite(spriteName);
                    }
                    
                    if(spritePaser != null)
                    {
                        if(!strUTF8.matches("^[1-9]\\d*$"))//不是整数 开始装换
                        {
                            animIndex = spritePaser.findIndex(AnimName);
                            //System.out.println(strUTF8 +" ==== " + animIndex);
                            
                            if(animIndex >= 0)//找到了
                            {
                                strUTF8 = String.valueOf(animIndex);
                            } else 
                            {
                                strUTF8 = "0";
                            }
                        }
                    }

                    
                    //System.out.println("strUTF8" + strUTF8);
                    if (VerifyCellDataNeedExport(null, SheetName, indexTable, col, row, strUTF8)) {
                        stateDetailMap.put(indexTable.get("" + col), strUTF8);
                    }

                    if (StateCol == col && IDCol != -1) {
                        // 将状态写入到iState文件中
                        stateBuffer = stateBuffer + "#define	" + strUTF8 + "		"
                                + curSheet.getCell(IDCol, row).getContents().toString().trim() + "\n";
                    }

                    if (CreateFilesCol == col && StateCol != -1 && strUTF8.equals("YES")) {
                        // String stateFileName = "output\\STATE_" +
                        // curSheet.getName() + "\\" +
                        // curSheet.getCell(StateCol,
                        // row).getContents().toString() + ".h";
                        String StateName = curSheet.getCell(StateCol, row).getContents().toString().trim();
                        createStateFile(curSheet.getName(), StateName);
                        stateFileIncludeFileBuffer.add(StateName);

                    }


                }

                if (row != 0) {
                    stateList.add(stateDetailMap);
                }
            }

            //记录iState.h,适用于Actor, hero, enemy
            if (StateCol != -1 && IDCol != -1) {
                stateBuffer = stateBuffer + "\n\n\n";
                if (CreateFilesCol != -1) {
                    createStateIncludeFile(curSheet.getName(), stateFileIncludeFileBuffer);
                }

            }


            curPageMap.put("table", stateList);
            
            JSonOutPutMap.put(SheetName, curPageMap);

            //curPageJson = JSONObject.fromObject(curPageMap);
            JSONObject printJson = JSONObject.fromObject(curPageMap);
            MainFrame.append("json Sheet <" + SheetName + "> is :" + printJson.toString());
            //raf.writeShort(s_playerNum);
            // = URLEncoder.encode(curPageJson.toString(), "UTF-8");//URLEncoder.encode(curPageJson, "UTF-8");

            //String strJason = strUTF8;


        }

        JSonOutPut = JSONObject.fromObject(JSonOutPutMap);
        //raf.writeBytes();
        //raf.writeBytes();
        if (isBase64) {
            String s = new sun.misc.BASE64Encoder().encode(JSonOutPut
                    .toString().getBytes("gb2312"));
            raf.write(s.getBytes());
        } else {
            raf.write(JSonOutPut.toString().getBytes("gb2312"));
        }
        //String tmp = JSonOutPut.toString();
        //raf.writeUTF(JSonOutPut.toString());


        String iStateFile = users.get("actorState", "iStateFiles");
        writeIStateFile(iStateFile, stateBuffer);


        raf.close();
        MainFrame.append("sheet Counter" + allSheet.length);




        MainFrame.append("数据处理完毕...");
        MainFrame.append("*******************************");
        MainFrame.append(" 角色状态属性表(sl.bin) " + MainFrame.getNowTime() + " 导出成功...");
        MainFrame.append("*******************************");
    }

    static boolean VerifyCellDataNeedExport(Map<String, Object> pagePropertyMap, String SheetName, Map<String, String> indexTable,
            int col, int row, String Data) throws Exception {
        if (Data.trim().equals("")) {

            /*if(pagePropertyMap.get("parent").equals("actor") || SheetName.equals("actor"))
             {
             if(indexTable.get("" + col).equals("ID") || indexTable.get("" + col).equals("State"))
             throw new Exception("导出失败：表<" + SheetName + ">第" + row + "行" + col + "列 [" + indexTable.get("" + col) + "]不能为空");
             }*/


            //MKFrame.append("警告：表<" + SheetName + ">中第" + (row - 1) + "行第" + col + "列  [" + indexTable.get("" + col) + "]为空");

            return false;
        }

        if (indexTable.get("" + col).equals("Description") || Data.equals("NULL") || Data.equals("NONE")) {
            return false;
        }
        //DescriptionForGD == col || strUTF8.equals("NONE") || strUTF8.equals("NULL") 


        return true;

    }

    //获得表属性
    static Map<String, Object> getPageProperty(Sheet curSheet) throws Exception {
        Map<String, Object> pMap = new HashMap<String, Object>();
        String SheetName = curSheet.getName();
        if (SheetName.equals("common_define")) {
            return null;
        }

        if (!curSheet.getCell(0, 0).getContents().toString().equals("property")) {
            MainFrame.append("错误：表<" + SheetName + ">第一行未添加属性行");
            //exportOK = false;
            throw new Exception("导出失败：表<" + SheetName + ">第一行未添加属性行");
            //return;
        }
        boolean hasSetType = false;
        for (int propCol = 1; propCol < curSheet.getRow(0).length; propCol += 2) {
            String Key = curSheet.getCell(propCol, 0).getContents().toString();
            String Property = curSheet.getCell(propCol + 1, 0).getContents().toString();
            if (Key.equals("")) {
                continue;
            }
            //验证父对象有效性
            if (Key.equals("type") && !Property.equals("")) {

                //有添些了Parent
                hasSetType = true;

            }

            pMap.put(Key, Property);

        }

        if (!hasSetType) {
            MainFrame.append("导出失败：表<" + SheetName + ">未指定类型");
            throw new Exception("导出失败：表<" + SheetName + ">未指定类型");
        }

        return pMap;

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

    //创建State文件
    static void createStateFile(String sheetName, String stateName) throws IOException {
        if (sheetName.equals("主角")) {
            sheetName = "hero";
        } else if (sheetName.equals("普通敌人")) {
            sheetName = "enemy";
        } else if (sheetName.equals("角色基础")) {
            sheetName = "actor";
        }

        String SheetNameUpperCase = sheetName.toUpperCase();
        String STATE_FILE_ROOT = users.get("actorState", "StateFilesRoot");
        String stateHFileName = "" + STATE_FILE_ROOT + "\\STATE_" + SheetNameUpperCase + "\\" + stateName + ".h";
        String stateHPPFileName = "" + STATE_FILE_ROOT + "\\STATE_" + SheetNameUpperCase + "\\" + stateName + ".hpp";
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

    static void createStateIncludeFile(String sheetName, ArrayList<String> stateList) throws IOException {
        if (sheetName.equals("主角")) {
            sheetName = "hero";
        } else if (sheetName.equals("普通敌人")) {
            sheetName = "enemy";
        } else if (sheetName.equals("角色基础")) {
            sheetName = "actor";
        }

        if (stateList.size() == 0) {
            return;
        }
        String STATE_FILE_ROOT = users.get("actorState", "StateFilesRoot");
        String SheetNameUpperCase = sheetName.toUpperCase();
        String stateHFileName = "" + STATE_FILE_ROOT + "\\" + sheetName + "Header.h";
        String stateHPPFileName = "" + STATE_FILE_ROOT + "\\" + sheetName + "Header.hpp";
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
            stringBuffer = stringBuffer + "#include \"./STATE_" + SheetNameUpperCase + "/" + stateList.get(i) + ".h\"\n";
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
            stringBuffer = stringBuffer + "#include \"./STATE_" + SheetNameUpperCase + "/" + stateList.get(i)
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
}