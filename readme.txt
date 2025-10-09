SerialReader,从串口读取string
StringExt,将string分割成命令及参数
command，处理命令及参数
LastSectionReaderWriter，读写flash
defense，防爆破方案：1.连续10次密码错误锁死，要频繁写flash；2.初始间隔1s，连续10次密码错误后间隔5s；用方案2.