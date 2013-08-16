# -*- coding: utf-8 -*-

import sys
import os
import shutil
from string import split
import tarfile
from types import ListType


# This expand_path is used to expand the path with your desired folders
expand_pathlist = []

if len(expand_pathlist) != 0:
    expand_path = os.pathsep + os.pathsep.join(expand_path)
else:
    expand_path = ''


class multios():
    operative_instructions = os.name
    search_path = os.environ['PATH'] + expand_path
    
    def copy(self, src, dst):
        """ Given a source will copy to destination and also it's meta, doesn't care if are folder or a file
        """
        if os.path.isdir(src) == True:
            shutil.copytree(src,dst)
        else:
            shutil.copy(src,dst)
        # Solo propagamos permisos en caso de que el destino no sea un directorio.
        if os.path.isdir(dst) == False:
            shutil.copystat(src,dst)
        
    def touch(self,filename):
        """ Given a file do a "touch" in a multiplataform way
        """
        try:
            os.utime(filename, None)
        except:
            open(filename, 'a').close()
            
    def mkdir_p(self,foldername,mode):
        """ Given a path create it, it will create intermediate folders
        """
        try:
            os.makedirs(foldername,mode)
        except OSError as exc: # Python >2.5
            if exc.errno == 17:
                pass
            else: raise
        
    def grep(self,word,filename):
        """ Given the word, search in the file
        """
        for line in open(filename):
         if word in line:
           return line
           
    def search_executable(self, filename):
       """ Given a search path, find file
       """
       file_found = 0
       paths = split(self.search_path, os.pathsep)
       for path in paths:
          if os.path.exists(os.path.join(path, filename)):
              file_found = 1
              break
       if file_found:
          return os.path.abspath(os.path.join(path, filename))
       else:
          return None
          
          
    def compress(self, src, tar_file_name):
        """ Given the source will compress it into a *.tar.gz
        """
        try:
            tarFile = tarfile.open(tar_file_name, mode = 'w:gz')
            if (type(src) == type([])):
                for item in src:
                    stripeditem = os.path.basename(item)
                    tarFile.add(item,stripeditem)
            else:
                tarFile.add(src)
            tarFile.close()
            return True
        except:
            return None
            
    def decompress(self, tar_file_name, dst):
        """ Given the source will decompress a *.tar.gz
        """
        try:
            tarFile = tarfile.open(tar_file_name, mode = 'r:gz')
            tarFile.extractall(dst)
            tarFile.close()
            return True
        except:
            return None

    def correctFileName(self, filename):
        """ In old Windows Operating Systems we can have problems with filenames and spaces
        """
        if os.name == 'nt':
           filename = "\"" + filename + "\""
        return filename

          
# Su only exists on posix systems
if os.name == 'posix':
    # LINUX - UNIX
    as_postgres = 'su postgres -c \"'
    as_postgres2 = "su postgres -c \'"
    end_sql = '\"'
    end_sql2 = "\'"
    psql = as_postgres + multios().search_executable("psql")
    psql2 = as_postgres2 + multios().search_executable("psql")
    pg_dump = multios().search_executable("pg_dump")
    createdb = as_postgres + "createdb" + ' -E UNICODE '
    dropdb = as_postgres + "dropdb "
    createuser = as_postgres + "createuser"
elif os.name=='nt':
    #Win XP
    as_postgres = ''
    as_postgres2 = as_postgres
    end_sql = ''
    end_sql2 = end_sql
    psql = "\"" + os.getcwd().replace("bulmasetup","pgsql\\bin\\psql.exe") + "\""
    print psql
    psql2 = "\"" + os.getcwd().replace("bulmasetup","pgsql\\bin\\psql.exe")  + "\""
    pg_dump = "\"" + os.getcwd().replace("bulmasetup","pgsql\\bin\\pg_dump.exe")  + "\""
    createdb = "\"" + os.getcwd().replace("bulmasetup","pgsql\\bin\\createdb.exe") +"\"" + " "
    dropdb = "\"" + os.getcwd().replace("bulmasetup","pgsql\\bin\\dropdb.exe") +"\"" + " "
    createuser = "\"" + os.getcwd().replace("bulmasetup","pgsql\\bin\\createuser.exe") + "\"" + " "
else:
    # Other Windows
    as_postgres = ''
    as_postgres2 = as_postgres
    end_sql = ''
    end_sql2 = end_sql
    psql = os.getcwd().replace("bulmasetup","pgsql\\bin\\psql.exe") 
    psql2 = os.getcwd().replace("bulmasetup","pgsql\\bin\\psql.exe") 
    pg_dump = os.getcwd().replace("bulmasetup","pgsql\\bin\\pg_dump.exe") 
    createdb = os.getcwd().replace("bulmasetup","pgsql\\bin\\createdb.exe") + " "
    dropdb = os.getcwd().replace("bulmasetup","pgsql\\bin\\dropdb.exe") + " "
    createuser = os.getcwd().replace("bulmasetup","pgsql\\bin\\createuser.exe") + " "
    


