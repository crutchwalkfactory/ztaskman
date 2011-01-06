//
// Project: zTaskMan
//
// C++ Implementation: 
//
// Description: utils functions
//
//
// Author: Ant-ON <prozanton@gmail.com>, (C) 2010-2011
//

#include <qpixmap.h>
#include <qimage.h>

#include <ZSkinTool.h>

void imgSetMaxSize( QPixmap * pm, int a )
{
	ZSkinTool tool;
	*pm=tool.process(*pm, a, a, spScale);
	
	/*
	if ( pm->height() > a || pm->width() > a )
	{
		QImage image;
		image = pm->convertToImage();
		image = image.smoothScale(a, a);
		pm->convertFromImage(image);	
	}
	*/
}

#include <iostream>
#include <fstream>

void toLog(QString mes)
{
	if (!mes.isEmpty())
		std::cout << QCString(mes) << std::endl;

}
