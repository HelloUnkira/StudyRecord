#!/usr/bin/env python3
# -*- coding:utf-8 -*-
 
import win32api
import win32con
import win32gui
import functools
import re
import os
import time


def image_name_compare(image_name1, image_name2):
    match_rule = re.compile('.*?_(.*?)_(.*?).jpg')
    match1 = match_rule.findall(image_name1)[0]
    match2 = match_rule.findall(image_name2)[0]
    if int(match1[0]) < int(match2[0]):
        return -1
    if int(match1[0]) > int(match2[0]):
        return +1
    if int(match1[1]) < int(match2[1]):
        return -1
    if int(match1[1]) > int(match2[1]):
        return +1
    return 0


def setWallpaper(image_path):
    key = win32api.RegOpenKeyEx(win32con.HKEY_CURRENT_USER,"Control Panel\\Desktop",0,win32con.KEY_SET_VALUE)
    win32api.RegSetValueEx(key, "WallpaperStyle", 0, win32con.REG_SZ, "2")
    win32api.RegSetValueEx(key, "TileWallpaper", 0, win32con.REG_SZ, "0")
    win32gui.SystemParametersInfo(win32con.SPI_SETDESKWALLPAPER,image_path, 1+2)


if __name__ == '__main__':
    filepath = os.path.split(os.path.realpath(__file__))[0] + '\meinv'
    image_list = os.listdir(filepath)
    image_list.sort(key=functools.cmp_to_key(image_name_compare))
    while True:
        for image_name in image_list:
            image_path = filepath + '\\' + image_name
            print(image_path)
            setWallpaper(image_path)
            time.sleep(10)
            

