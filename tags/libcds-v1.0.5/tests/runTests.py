#!/usr/bin/python 

# runTests.py
# Copyright (C) 2010, Francisco Claude, all rights reserved.
#
# Francisco Claude <fclaude@cs.uwaterloo.ca>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

import sys,os,random,pickle,re

seed = ["4","7","21","9","3","1","12","25","8","10"]

array_len  = ["1", "2", "1000", "71", "1000003","19342531"]
array_maxv = ["100", "100", "10", "71", "100000","99"]
for i in range(len(array_len)):
    print("* running ./testArray "+seed[i]+" "+array_len[i]+" "+array_maxv[i])
    res=os.system("./testArray "+seed[i]+" "+array_len[i]+" "+array_maxv[i])
    if res!=0:
        print("./testArray "+seed[i]+" "+array_len[i]+" "+array_maxv[i]+" FAILED")

bitmap_len = ["0","1","2","10000","1000023","10000853"]
bitmap_den = ["0","0","1","641","5312","123517"]
for i in range(len(bitmap_len)):
    print("* running ./testBitSequence "+seed[i]+" "+bitmap_len[i]+" "+bitmap_den[i])
    res=os.system("./testBitSequence "+seed[i]+" "+bitmap_len[i]+" "+bitmap_den[i])
    if res!=0:
        print("./testBitSequence "+seed[i]+" "+array_len[i]+" "+array_maxv[i]+" FAILED")

array_len  = ["1", "2", "1000", "71", "1000003","19342531"]
array_maxv = ["100", "100", "10", "71", "100000","99"]
for i in range(len(array_len)):
    print("* running ./testSequence "+seed[i]+" "+array_len[i]+" "+array_maxv[i])
    res=os.system("./testSequence "+seed[i]+" "+array_len[i]+" "+array_maxv[i])
    if res!=0:
        print("./testSequence "+seed[i]+" "+array_len[i]+" "+array_maxv[i]+" FAILED")


