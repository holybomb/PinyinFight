package com.gc.hunter.tools;

import java.io.File;

import jxl.Sheet;
import jxl.Workbook;

public class ExcelUtil {
	public static final String DEFAULT_STRING = "";
	public static final int	DEFAULT_INT = 0;
	public static final double DEFAULT_DOUBLE = 0.0;

	public static Workbook getWorkBook(String XlsFileName) throws Exception {
		Workbook workbook = null;
		try {
			workbook = Workbook.getWorkbook(new File(XlsFileName));
		} catch (Exception e) {
			throw new Exception(XlsFileName + " file to import not found!");
		}
		return workbook;
	}

	public static boolean check(String Data) {
		if (Data.trim().equals("")) {
			return false;
		}

		if (Data.equals("NULL") || Data.equals("NONE")) {
			return false;
		}
		return true;
	}
	
	public static String getString(Sheet sheet, int col, int row)
	{
		String value = sheet.getCell(col, row).getContents().toString().trim();
		String valueStr = DEFAULT_STRING;
		if (check(value))
		{
			valueStr = value;
		}
		return valueStr;
	}
	
	public static int getInt(Sheet sheet, int col, int row)
	{
		String value = sheet.getCell(col, row).getContents().toString().trim();
		int valueInt = DEFAULT_INT;
		if (check(value))
		{
			valueInt = Integer.parseInt(value);
		}	
		return valueInt;
	}
	
	public static double getDouble(Sheet sheet, int col, int row)
	{
		String value = sheet.getCell(col, row).getContents().toString().trim();
		double valueDouble = DEFAULT_DOUBLE;
		if (check(value))
		{
			valueDouble = Double.parseDouble(value);
		}	
		return valueDouble;
	}
}
