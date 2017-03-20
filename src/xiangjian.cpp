#include "xiangjian.h"
#include "cmath"
XiangJian::XiangJian()
{
	//初始化算法名称
	arithName = "图片相减"; 
}

void XiangJian::setParam(const QList<QImage> &dataList, 
					  const QList<QString> &paramList)
{    
	//初始化数据
	sourceImageA = dataList[0]; 
	sourceImageB = dataList[1];

	//初始化参数
}

void XiangJian::dealData()
{
	QImage image(sourceImageA.size(), sourceImageA.format());

	int height = image.height();
	int width = image.width();
	for ( int w = 0; w != width; ++w)
	{
		for ( int h = 0; h != height; ++h)
		{
			int pixelA = sourceImageA.pixel(w,h);
			int pixelB = sourceImageB.pixel(w,h);
			int finalPixel =abs( pixelA - pixelB);
			//int gray = qGray( pixel);
			//gray = gray + (gray<<8) + (gray<<16) + (qAlpha(pixel)<<24);
			image.setPixel(w, h, finalPixel); 
		}
	}

	finalDataList.push_back(image); 
}
