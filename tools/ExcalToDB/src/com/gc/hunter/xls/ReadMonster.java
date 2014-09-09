package com.gc.hunter.xls;

import java.io.File;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gc.hunter.MainFrame;
import com.gc.hunter.tools.IniEditor;

import jxl.Sheet;
import jxl.Workbook;
import net.sf.json.JSONObject;

public class ReadMonster {

    public static String sheetNameArray[];
    public static final String ConfigName = "config.ini";
    public static boolean isBase64;
    static IniEditor users;

    public ReadMonster(boolean isUseBase64) throws Exception {
        isBase64 = isUseBase64;
        users = new IniEditor();
        users.load(ConfigName);
        String XlsFileName = users.get("monster", "sheetFiles");


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

        String binFileName = users.get("monster", "binFiles");
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


            Sheet curSheet = allSheet[page];
            String SheetName = curSheet.getName();
            //忽略空页和help页
            if (curSheet == null || curSheet.getName().equals("帮助") || curSheet.getName().equals("common_define")) {
                continue;
            }

            Map<String, Object> curPageMap = new HashMap<String, Object>();
            //Map<String, Object> 
            List<Object> stateList = new ArrayList<Object>();


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
                    //System.out.println("strUTF8" + strUTF8);
                    if (VerifyCellDataNeedExport(null, SheetName, indexTable, col, row, strUTF8)) {
                        stateDetailMap.put(indexTable.get("" + col), strUTF8);
                    }



                }

                if (row != 0) {
                    stateList.add(stateDetailMap);
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
        if (isBase64) {
            String s = new sun.misc.BASE64Encoder().encode(JSonOutPut.toString().getBytes("gb2312"));
            raf.write(s.getBytes());
        } else {
            raf.write(JSonOutPut.toString().getBytes("gb2312"));
        }


        raf.close();
        MainFrame.append("sheet Counter" + allSheet.length);



        MainFrame.append("数据处理完毕...");
        MainFrame.append("*******************************");
        MainFrame.append(" 怪属性表(ms.bin) " + MainFrame.getNowTime() + " 导出成功...");
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
}