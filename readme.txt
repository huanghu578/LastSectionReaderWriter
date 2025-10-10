一、SerialReader,从串口读取string。
帧间隔方案有两种：1.用\n or \r；2.用固定时间阈值。方案1强制要求用户输入帧间隔，用户体验不好，选用方案2。
另外，因为只希望从串口获取可见字符（包括空格），所有不可见字符丢弃。
二、StringExt,将string分割成命令及参数
三、command，处理命令及参数
四、LastSectionReaderWriter，读写flash
五、defense，防爆破方案：1.连续10次密码错误锁死，要频繁写flash；2.初始间隔1s，连续10次密码错误后间隔5s；用方案2.