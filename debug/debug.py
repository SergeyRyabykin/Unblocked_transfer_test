import subprocess
import time
import sys

dbgScript = sys.argv[1]

openocd = subprocess.Popen("openocd -f interface/stlink-v2-1.cfg -f target/stm32f1x.cfg")
time.sleep(5)
debugger = subprocess.Popen("gdb-multiarch -x " + dbgScript)


debugger.wait()
openocd.terminate()
