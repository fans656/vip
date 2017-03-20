#pragma once
//QString savedFormat( "png");
const int W_ICONSIZE = 105;					//图片宽度
const int H_ICONSIZE = 125;					//图片高度
const int H_ITEMSIZE = 145;					//单元项高度(因为单元项既包括图片又包括文本)
const int W_ITEMSIZE = 130;
static const double maxerFactor = 1.25;		//放大倍数
static const double minnerFactor = 0.8;		//缩小倍数
enum DataType {ImageType = 1, GraphicType, MultidataType, SymbolType, VidioType};  //数据类型
