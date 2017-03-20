#include "togray.h"

ToGray::ToGray()
{
    //初始化算法名称
    arithName = "图像灰度化"; 
}

void ToGray::setParam(const QList<QImage> &dataList, 
                      const QList<QString> &paramList)
{    
    //初始化数据
    sourceImage = dataList.first(); 
    
    //初始化参数
}

void ToGray::dealData()
{
    QImage image(sourceImage.size(), sourceImage.format());
    
    int height = image.height();
    int width = image.width();
    for ( int w = 0; w != width; ++w)
    {
        for ( int h = 0; h != height; ++h)
        {
            int pixel = sourceImage.pixel(w,h);
            int gray = qGray( pixel);
            gray = gray + (gray<<8) + (gray<<16) + (qAlpha(pixel)<<24);
            image.setPixel(w, h, gray); 
        }
    }
    emit 
       
    finalDataList.push_back(image); 
}
