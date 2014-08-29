#!/usr/bin/python
#-*- coding: utf-8 -*-

'''
Created on 2014-5-29

AES加密资源
MD5资源名字
'''
import os
import hashlib

#定义 LOG信息到屏幕和txt
def loginfo(message):
    print(message)
    logfile.write('  ')
    logfile.write(str(message))
    logfile.write('\n')

def removeFileInFirstDir(targetDir): 
    for root, dirs, files in os.walk(targetDir, True):
        for name in files:
            if root.find('.svn') < 0:
                name = root + "/" + name
                #loginfo("del file : " + name)
                os.remove(name)

#定义 os walk遍历所有文件
def walk_dir(indir, outdir):
    for root, dirs, files in os.walk(indir, True):
        #处理lim并拷贝,拷贝其他文件
        for name in files:
            if root.find('.svn') < 0:
                namesuff = os.path.splitext(name)

                inname = root + "/" + name
                AES=False
                if namesuff[1]==".png" or namesuff[1]==".fsh" or namesuff[1]==".vsh" or namesuff[1]==".glsl" or namesuff[1]==".frag" or namesuff[1]==".vert":
                    AES=True
                #md5 name
                if AES:
                    name=name+".li"

                #loginfo("name:  " + name)    
                hash_md5 = hashlib.md5(name)

                dirlen = len(indir)
                outname = outdir + root[dirlen:]

                if not os.path.exists(outname): 
                    os.makedirs(outname) 

                #lua 不用MD5
                if namesuff[1]==".lua":
                    outname = outname + "/" + name
                else:
                    outname = outname + "/" + hash_md5.hexdigest()[8:-8]

                inname = inname.replace('\\', '/')
                outname = outname.replace('\\', '/')

                loginfo("inname:  " + inname)
                loginfo("outname:  " + outname)

                if AES:
                    os.system(tool + " " + inname + " " + outname + " " + password)
                else:
                    open(outname, "wb").write(open(inname, "rb").read()) 


#入口
if  __name__ =="__main__":

    logfile = open('log_make_res.txt', 'w')

    os.chdir('..')
	
    #path
    trunk = os.getcwd()
    loginfo("trunk : " + trunk)
    inFile=os.path.join(trunk, "projectHunter/Resources")
    outFile=os.path.join(trunk, "projectHunter/ResourcesMD5")
    tool=os.path.join(trunk, "batch/tools/processRes.exe")

    password='0123456789abcde'

    loginfo('--Start')
    loginfo("outFile : " + outFile)
    removeFileInFirstDir(outFile)
    walk_dir(inFile, outFile)

    loginfo('--Success')