SerialReader,从串口读取string。如果用\n or \r作为帧间隔，就会强制要求用户输入帧间隔，这样用户体验不好。改用字符间隙来作为帧间隔，用线程读取输入。
StringExt,将string分割成命令及参数
command，处理命令及参数
LastSectionReaderWriter，读写flash
defense，防爆破方案：1.连续10次密码错误锁死，要频繁写flash；2.初始间隔1s，连续10次密码错误后间隔5s；用方案2.