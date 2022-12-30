 #!/usr/bin/env python3
""" 
  # DISCLAIMER

  Created by Manuel Schreiner
 
  Copyright © 2022 io-expert.com. All rights reserved.
 
  1. Redistributions of source code must retain the above copyright notice,
     this condition and the following disclaimer.
 
  This software is provided by the copyright holder and contributors "AS IS"
  and any warranties related to this software are DISCLAIMED.
  The copyright owner or contributors be NOT LIABLE for any damages caused
  by use of this software.
"""

import requests
import json
from requests.auth import HTTPBasicAuth
import os
import sys
import argparse

def flashViaComPort(port,filepath,mcu):
    if mcu == "esp32":
        os.system(F"python3 -m esptool --port {port} --baud 115200 write_flash -z 0x1000 {filepath}")
    elif mcu == "esp8266":
        os.system(F"python3 -m esptool --port {port} --baud 115200 write_flash 0x0 {filepath}")

def flashViaWeb(url,auth,filepath):
    with open(filepath, 'rb') as fobj:
        headers = {"Content-Type": "application/octet-stream"}
        file_basename = os.path.basename(filepath)
        file_to_upload = {"update": (str(file_basename), fobj)}
        finfo = {"fullPath": filepath}
        response = requests.post(url, auth=basicauth, files=file_to_upload)
        fobj.close()
    print("Status Code", response.status_code)
    print(response.text)



parser = argparse.ArgumentParser(
                    prog = 'upload.py',
                    description = 'Upload files via http POST to ESP',
                    epilog = '(c) Manuel Schreiner - IO-Expert.com')

parser.add_argument('filename') 
parser.add_argument('-u', '--user',default='admin',required=False)
parser.add_argument('-p', '--password',default='admin',required=False)
parser.add_argument('-b', '--url',default='http://my-esp.local/update',required=False)
parser.add_argument('-c', '--comport',default='com1',required=False)
parser.add_argument('-i', '--interface',default='web',required=False,choices=['web', 'comport'])
parser.add_argument('-m', '--mcu',default='esp8266',required=False,choices=['esp8266', 'esp32'])


args = parser.parse_args()

basicauth = HTTPBasicAuth(args.user, args.password)
url = args.url
filepath=args.filename
comport=args.comport
mcu=args.mcu

if args.interface == "web":
    flashViaWeb(url,basicauth,filepath)
elif args.interface == "comport":
    flashViaComPort(comport,filepath,mcu)


    
