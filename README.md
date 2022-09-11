# RobotTeamwork

这是浙江大学2022年9月机器人基础实践课程代码。

本仓库在9.8课程结束之前仅作为组内代码托管、协作开发平台。
在9.8课程结束之前，请勿随意传播本仓库。

robot.ino 依赖以下库:

```cpp
<NewPing.h>
<TB6612.h>
<GreyScaleSensor.h>
<RGB.h>
<ColorDetector.h>
```

其中除`NewPing.h`外，所用库的代码均包含在了`robot/libs`中。

若想要在本地编译程序，需要将`robot/libs`添加到 Arduino IDE 的 `libraries` 中。
