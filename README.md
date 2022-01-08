### 简介

该项目示例参考LearnOpenGL的官方示例进行编写，网址：[GItHub](https://github.com/JoeyDeVries/LearnOpenGL)

学习的参考文档为： [文档](https://learnopengl-cn.github.io/)





使用环境平台相关

* Mac  Version: 10.14.6
* Xcode Version: 11.3.1



工具相关使用的是： **GLView**,  从AppStore搜索下载



### 其他

关于官网示例， Mac Xcode 配置相关命令：

* 下载LearnOpenGL，[GitHub](https://github.com/JoeyDeVries/LearnOpenGL)
* 通过终端进入指定的目录,运行命令：

```
brew install cmake assimp glm glfw
mkdir build
cd build
cmake -G "Xcode" ..
```

* 如果cmake -G报错，类似提示：

```
xcode-select: error: tool 'xcodebuild' requires Xcode, but active developer directory '/Library/Developer/CommandLineTools' is a command line tools instance
```

修改方式：

1. 执行命令,切换Xcode路径

```
sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer/
```

2. 执行命令

```
xcodebuild -showsdks
```

3. 执行命令

```
xcrun --sdk iphoneos --show-sdk-path
```

4. 显示出类似结果，表示正确

```
/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS11.2.sdk
```

