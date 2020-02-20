工程配置
===============

一、准备工作<br>
--------------------
OS: Windows 7及以上版本<br>
Cmake: 3.6以上版本<br>

二、生成工程文件<br>
---------------------
1.将文件夹XJGARFaceSDKProjectDemo-win64导入到本地，本例中，将文件放在D盘根目录下。<br>
2.打开CMAKE，设定好OpenCV目录等参数，点击Configure进行工程参数配置，然后点击Generate生成解决方案，具体的参数设置下图 所示。 <br>
![image](https://github.com/xiaojigou/XJGARSDKDemoApp-Win64/blob/master/ImageCache/1.png)<br>
3. 利用Visual Studio打开解决方案：libXJGARSDKTest.sln。<br>
![image](https://github.com/xiaojigou/XJGARSDKDemoApp-Win64/blob/master/ImageCache/2.png)<br>
4.复制OpenCV的dll文件到解决方案目录下，如<br>
![image](https://github.com/xiaojigou/XJGARSDKDemoApp-Win64/blob/master/ImageCache/3.png)<br>
5.生成解决方案，然后设置项目libXJGARSDKTest为启动项目，点击运行项目即可<br>
![image](https://github.com/xiaojigou/XJGARSDKDemoApp-Win64/blob/master/ImageCache/4.png)<br>


#免费License申请、贴纸制作工具地址
====================
http://www.xiaojigou.cn 首页-》开发包-》说明文档-》XJGARSDK说明文档

#API接口：<br>
=====================
注意：SDK中各个函数需要在单一的线程中调用。<br>
1.	初始化<br>
>####初始化方法：<br>
XJGARSDK_API bool XJGARSDKInitialization(const char* licenseText, 
	const char* userName, const char* companyName);<br>
* 第一个参数为key<br>
* 第二个参数为 key对应的用户名<br>
* 第三个参数为 key对应的公司名<br>
该参数须申请<br>

>####销毁方法：<br>
XJGARSDKCleanUP();<br>

2.	使用人脸整形<br>
>####大眼：<br>
XJGARSDK_API bool XJGARSDKSetBigEyeParam(int eyeParam);<br>
eyeParam参数为0-100，数值越大眼睛越大<br>

>####瘦脸：<br>
XJGARSDK_API bool XJGARSDKSetThinChinParam(int chinParam); <br>
chinParam参数为0-100，数值越大脸部下吧越瘦<br>

>####红润：<br>
XJGARSDK_API bool XJGARSDKSetRedFaceParam(int redFaceParam); <br>
redFaceParam参数为0-100，数值越大脸部皮肤越红润<br>

>####美白：<br>
XJGARSDK_API bool XJGARSDKSetWhiteSkinParam(int whiteSkinParam); <br>
whiteSkinParam参数为0-100，数值越大脸部皮肤越白<br>

>####磨皮：<br>
XJGARSDK_API bool XJGARSDKSetSkinSmoothParam(int skinSmoothParam); <br>
skinSmoothParam参数为0-100， 数值越大越皮肤越光滑<br>

3.	使用人脸滤镜<br>
SDK启动时默认不使用滤镜<br>
>####切换滤镜：<br>
XJGARSDK_API bool XJGARSDKChangeFilter(const char*  filterTypeName);<br>
filterTypeName参数为滤镜名字，目前可选的滤镜有6种，分别是冰冷,健康,祖母绿,怀旧, 蜡笔, 常青，填入“无”不使用滤镜;<br>
在某些中文输入有问题的状况下可以使用英文参数输入，6种滤镜分别为："filter_cool", <br>"filter_Healthy","filter_emerald","filter_nostalgia","filter_crayon", "filter_evergreen"。填入"filter_none",不使用滤镜。<br>

4.	使用人脸道具<br>
>####显示贴纸：<br>
XJGARSDK_API bool XJGARSDKSetShowStickerPapers(bool bShowStickPaper);<br>
bShowStickPaper参数 为true时，显示贴纸<br>

>####切换贴纸：<br>
XJGARSDK_API bool XJGARSDKChangeStickpaper(const char*  stickPaperName);<br>
stickPaperName参数为贴纸名称，目前可选的贴纸见StickerPapers子文件夹，每个文件夹的名称均是贴纸名称<br>
5.	图片视频流处理<br>
>//初始化OpenGL 环境<br>
>XJGARSDK_API bool XJGARSDKInitOpenglEnvironment(int width,	int height);<br>
>///if user dont't have opengl environment, call this to set up a virtual opengl environment<br>
>///@width:	width of input image<br>
>///@height: height of input image<br>
>///@return  true: success, false: fail<br>

>///销毁OpenGL环境<br>
>XJGARSDK_API bool XJGARSDKDestroyOpenglEnvironment();<br>
>///释放OpenGL资源<br>
>XJGARSDK_API bool XJGARSDKReleaseAllOpenglResources();<br>

>//设置SDK工作模式，0为视频，1为图片<br>
>XJGARSDK_API bool XJGARSDKSetOptimizationMode(int mode);<br>
>///set different optimization mode for video or image<br>
>///@mode:	optimization mode, 0: video, 1: image<br>

>XJGARSDK_API int XJGARSDKRenderImage(const unsigned char* image, int width,<br>
>	int height, unsigned char* imageBufOut);<br>
>前三个是入参，其中image参数是图像RGB数据，width 图像宽带，heifht图像高度<br>
>imageBufOut为出参，即经过美颜，滤镜，道具处理后的图像，可用OpenCV的imshow函数直接播放。或者也可以采用doublebuffer方案自行写播放函数<br>

>XJGARSDK_API int XJGARSDKRenderImage(const unsigned char* image, int width,<br>
>	int height);<br>
>其中image参数是图像RGB数据，width 图像宽带，heifht图像高度<br>
>调用该函数后，将在Opengl屏幕缓存中存储最后渲染的结果。<br>

>XJGARSDK_API int XJGARSDKRenderImage(const unsigned char* image, int width,<br>
>	int height, int* pOutputTexId);<br>
>///input 3 channels RGB image, render to get result opengl texture name<br>
>///@width:	width of input image<br>
>///@height: height of input image<br>
>///@outputTexId:	result opengl texture name<br>

>XJGARSDK_API int XJGARSDKRenderGLTexture(int inputTexId, int width, <br>
>	int height, int* pOutputTexId);<br>
>///input opengl texture , render to get result opengl texture <br>
>///@inputTexId:input opengl texture<br>
>///@width:	width of input texture<br>
>///@height: height of input texture<br>
>///@outputTexId:	result opengl texture name<br>

>XJGARSDK_API int XJGARSDKRenderGLTexture(int inputTexId, int width, int height);<br>
>///input opengl texture , render to opengl back buffer<br>
>///@inputTexId:input opengl texture<br>
>///@width:	width of input texture<br>
>///@height: height of input texture<br>

>XJGARSDK_API void XJGARSDKDrawAFullViewTexture(int inputTexId, int startX, int startY, int viewportWidth, int viewportHeight);<br>
>///Given a inputeTexId, draw to target opengl viewport<br>
>///@inputTexId: the inpute text id<br>
>///@startx: start x coordinates of view<br>
>///@startY: start y coordinates of view<br>
>///@viewportWidth: width of view<br>
>///@viewportWidth: height of view<br>

>XJGARSDK_API int XJGARSDKGetTargetResultImgAndLandMarks(unsigned char* imageBufOut, int* pOutputTexId, float* faceLandmarks, int >targetWidth, int targetHeight, int iImgCropMode, float *pXscale = 0, float * pYScale = 0);<br>
>//在每一帧渲染结束后，可以使用该函数获取结果图像及脸部特征点，<br>
>imageBufOut为结果图像存储的RGB缓存，如果为空，则不输出rgb图像。<br>
>pOutputTexId为结果图像存储的纹理Id，如果为空，则不输出纹理对象。<br>
>以上两个参数至少有一个不能为空。<br>
>faceLandmarks为结果脸部特征点缓存的数组缓存。格式为：第一个人的脸部特征点列表，第二个人脸部特征点列表…<br>
>targetWidth:指定输出rgb图像，或纹理的宽度<br>
>targetHeight：指定输出rgb图像，或纹理的高度<br>
>iImgCropMode：目标图像与相机输入的目标图像尺寸不一致时，指定输入目标图像的裁剪方式，0：拉伸到目标位置，1:中心点对齐并裁剪边界（不缩放），2：居中对齐并缩放以尽可能匹配目标，然后裁剪<br>
>///@pXscale, 捕获的结果图像坐标与原始图像坐标的缩放参数<br>
>///@pYScale, 捕获的结果图像坐标与原始图像坐标的缩放参数<br>
>返回值：检测到的人脸个数<br>
![image](https://github.com/TeacherLuo/XJGARSDKDemoApp-Win64/raw/master/ImageCache/5.png)<br>
>//在每一帧渲染结束后，可以使用该函数获取原始图像及脸部特征点，<br>
>XJGARSDK_API int XJGARSDKGetFaceLandmarks(float* faceLandmarks);<br>

