# -*- coding: utf-8 -*-

import sys
import os
import shutil

# This expand_path is used to expand the path with your desired folders
expand_pathlist = []
if len(expand_pathlist) != 0:
    expand_path = os.pathsep + os.pathsep.join(expand_path)
else:
    expand_path = ''
if os.name == 'posix':
    as_postgres = 'su postgres -c \"'
    end_sql = '\"'
else:
    as_postgres = ""
    end_sql = ""

class multios():
    operative_instructions = os.name
    search_path = os.environ['PATH'] + expand_path
    
    def copy(self, src, dst):
        if os.path.isdir(src) == True:
            shutil.copytree(src,dst)
        else:
            shutil.copy(src,dst)
        shutil.copystat(src,dst)
        
    def touch(self,fname):
        try:
            os.utime(fname, None)
        except:
            open(fname, 'a').close()
            
    def mkdir_p(self,desiredpath,mode):
        try:
            os.makedirs(desiredpath,mode)
        except OSError as exc: # Python >2.5
            if exc.errno == 17:
                pass
            else: raise
        
    def grep(self,word,desiredpath):
        for line in open(desiredpath):
         if word in line:
           return line
