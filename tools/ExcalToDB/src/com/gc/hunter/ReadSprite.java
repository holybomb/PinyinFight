package com.gc.hunter;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class ReadSprite {
	
	
	HashMap<String, Integer> AnimIndexMap;
	
	
	public ReadSprite()
	{
		AnimIndexMap = new HashMap<String, Integer>();
	}
	
	
	public static void main(String[] args) {
		
		ReadSprite rs = new ReadSprite();
		
		rs.paseSprite("z:\\hero1.sprite");
		
		System.out.println("walk + "+rs.findIndex("Walk"));
		
		System.out.println("3 + "+rs.findAnimName(3));

	}
	
	
	public void paseSprite(String spriteName)
	{
				
		File dic = new File(spriteName);
		
		BufferedReader br = null;
		
		try {
			
			br = new BufferedReader(new FileReader(dic));
			
			int AnimIndex = 0;
			Pattern pattern = Pattern.compile("^\\s+ANIM*",Pattern.CASE_INSENSITIVE);
			
			Pattern patternName = Pattern.compile("\"\\w+\"",Pattern.CASE_INSENSITIVE);
			
			Pattern patternIndex = Pattern.compile("// Index =\\s*\\d*,",Pattern.CASE_INSENSITIVE);
			
			String line = br.readLine();
	
		
			do{
				
				Matcher matcher = pattern.matcher(line);
				
				if(matcher.find())
				{
					matcher = patternName.matcher(line);
					if(matcher.find())
					{
						int start = matcher.start();
						int end =  matcher.end();
						if(end - start > 2)
						{
							String name = line.substring(start + 1,end - 1);
							
							
							matcher = patternIndex.matcher(line);
							
							if(matcher.find())
							{
								start = matcher.start();
								end =  matcher.end();
								
								String index = line.substring(start + "// Index =".length(), end -1);
								
								int indexNumb = Integer.parseInt(index.trim());
								
								//System.out.println(name +"--   --"+ indexNumb);
								
								if(AnimIndex != indexNumb)
								{
									System.out.println("check anim index are corr");
								}
								
								AnimIndexMap.put(name.toLowerCase(), indexNumb);
							}
						}else
						{
							System.out.println("!! error  did NOT has name for ANIM:" + line);
						}
					}
					AnimIndex ++;
					
				}
				
				
				line = br.readLine();
			}while(line != null);
			
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				if(br != null)
					br.close();
			} catch (IOException e) {
			}
		}
	}
	
	
	public int findIndex(String name)
	{
            int index = -1;
            
            try{
                index = AnimIndexMap.get(name.toLowerCase());
            }catch(Exception ex)
            {  }
            return index;
		
	}
	
	public String findAnimName(int index)
	{
		Set<String> key = AnimIndexMap.keySet();
		
		for(Iterator<String> it = key.iterator(); it.hasNext();)
		{
			String s = it.next();
			if(AnimIndexMap.get(s) == index)
			{
				return s;
			}
		}
		
		return null;
	}

}
