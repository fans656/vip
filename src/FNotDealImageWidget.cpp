#include "FNotDealImageWidget.h"
#include "FImageRankDialog.h"
#include <QCursor>
#include <QMenu>
#include <QSettings>
#include <QDebug>
#include "stdafx.h"

FNotDealImageWidget::FNotDealImageWidget()
{
    GroupNum = 0;
    picSum = 0;
    notDealImageWidgetLayout();
    initNotDealImageWidget();
}

void FNotDealImageWidget::openImage()
{
    // 恢复上次打开文件的路径
    QSettings settings("config.ini", QSettings::IniFormat);
    if(!QFileInfo(settings.value("lastOpenDirPath").toString()).exists())
    {
        settings.setValue("lastOpenDirPath", QDir::currentPath());
    }
    QString dir = settings.value("lastOpenDirPath").toString();
    QString filter = "Images (*.png *.jpg *.ico *.jpeg *.bmp);;All Files (*.*)";
    // 获取打开文件名列表
    QList<QString> paths = QFileDialog::getOpenFileNames(this, tr("打开图片"), dir, filter);
    
    for (QList<QString>::iterator iter = paths.begin(); 
         iter != paths.end(); iter++)
    {
        *iter = QFileInfo(*iter).absoluteFilePath(); 
    }
    
    if (!paths.isEmpty())
    {
        openImage(paths);
        emit imageOpened(paths);
        // 保存本次打开文件的路径
        QString currentDir = QFileInfo(paths.first()).absolutePath();
        settings.setValue("lastOpenDirPath", currentDir);
    }
}

void FNotDealImageWidget::openImage(QList<QString> paths)
{
    foreach (QString path, paths)
    {
        QFileInfo fileInfo(path);
        QString imageName = fileInfo.fileName();
        if (alreadyOpened(path))
        {
            emit hasOutput(tr("图片\"") + path + tr("\"打开失败，图片名称已经存在"));
            if (isContinueOpenWhenExist(path))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            emit hasOutput(tr("图片\"") + path + tr("\"打开成功"));
            openedImages.insert(path);
            
            // 向“单张图”控件添加item
            QListWidgetItem *item = new QListWidgetItem(imageName);
            item->setCheckState(Qt::Unchecked);
            item->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));		//设置单元项的宽度和高度
            item->setData(Qt::UserRole, path);
            
            // 将图像缩放操作指派给线程
            Worker *worker = new Worker(item, path);
            connect(worker, SIGNAL(iconCreated(QListWidgetItem*,QImage)), this, SLOT(setIcon(QListWidgetItem*,QImage)));
            Thread::run(worker);
        }
    }
}

bool FNotDealImageWidget::isContinueOpenWhenExist(const QString &imageName)
{
    QMessageBox dialog(QMessageBox::Warning, tr("Error"),
                       tr("图片") + imageName + tr("打开失败，图片名称已经存在"));
    dialog.addButton(tr("继续"), QMessageBox::AcceptRole);
    dialog.addButton(tr("取消"), QMessageBox::RejectRole);
    return (dialog.exec() == QMessageBox::AcceptRole ? true : false);
}

void FNotDealImageWidget::notDealImageWidgetLayout()
{
    selectAllButton = new QPushButton(tr("全选"),this);
    selectAllButton->setFixedSize(55,35);
    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));
    
    deleteImageButton = new QPushButton(tr("删除"),this);
    deleteImageButton->setFixedSize(55,35);
    connect(deleteImageButton,SIGNAL(clicked()),this,SLOT(deleteImage()));
    
    dealImageButton = new QPushButton(tr("处理"),this);
    dealImageButton->setFixedSize(55,35);
    connect(dealImageButton,SIGNAL(clicked()),this,SLOT(dealImage()));
    
    showImageButton = new QPushButton(tr("显示"),this);
    showImageButton->setFixedSize(55,35);
    connect(showImageButton,SIGNAL(clicked()),this,SLOT(showImage()));
    
    combineOrRankButton = new QPushButton(tr("组合"),this);
    combineOrRankButton->setFixedSize(55,35);
    //通过judgeCR函数，即判断Combine或者Rank函数来选择执行
    connect(combineOrRankButton,SIGNAL(clicked()),this,SLOT(judgeCR()));
    
    notDealImageList = new QListWidget;
    //notDealImageList->setFixedSize(300,270);
    notDealImageList->setMovement(QListView::Static);
    connect(notDealImageList,SIGNAL(itemClicked(QListWidgetItem *)),
            this,SLOT(notDealImageItemClickSlot(QListWidgetItem *)));
    connect(notDealImageList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,SLOT(onItemDoubleClicked(QListWidgetItem *)));
    
    notDealMoreImageList = new QListWidget;
    //notDealMoreImageList->setFixedSize(300,270);
    notDealMoreImageList->setMovement(QListView::Static);
    connect(notDealMoreImageList,SIGNAL(itemClicked(QListWidgetItem *)),
            this,SLOT(notDealMoreImageItemClickSlot(QListWidgetItem *)));
    connect(notDealMoreImageList,SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this,SLOT(notDealMoreWidgetItemDoubleClickedSlot(QListWidgetItem *)));
    
    notDealImageToolBox = new QToolBox(this);
    //notDealImageToolBox->setFixedSize(303,340);
    notDealImageToolBox->addItem(notDealImageList,"单张图");
    notDealImageToolBox->addItem(notDealMoreImageList,"多张图");
    connect(notDealImageToolBox, SIGNAL(currentChanged(int)), this, SLOT(toolBoxCurrentChangedSlot(int )));
    
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(selectAllButton);
    hlayout1->addWidget(deleteImageButton);
    hlayout1->addWidget(showImageButton);
    hlayout1->addWidget(combineOrRankButton);
    hlayout1->addWidget(dealImageButton);
    
    
    QVBoxLayout *vlayout3 = new QVBoxLayout;
    vlayout3->addLayout(hlayout1);
    vlayout3->addWidget(notDealImageToolBox);
    
    QVBoxLayout *notDealImageLayout = new QVBoxLayout;
    notDealImageLayout->addLayout(vlayout3);
    this->setLayout(notDealImageLayout);
}
void FNotDealImageWidget::initNotDealImageWidget()
{
    notDealImageList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    notDealMoreImageList->setSelectionMode(QAbstractItemView::SingleSelection);
    
    notDealImageList->setIconSize(QSize(W_ICONSIZE, H_ICONSIZE));
    notDealImageList->setResizeMode(QListView::Adjust);
    notDealImageList->setViewMode(QListView::IconMode);  //设置QListWidget的显示模式
    notDealImageList->setMovement(QListView::Static);    //设置QListWidget中的单元项不可被拖动
    notDealImageList->setSpacing(1);                     //设置QListWidget中的单元项的间距
    
    notDealMoreImageList->setIconSize(QSize(W_ICONSIZE, H_ICONSIZE));
    notDealMoreImageList->setResizeMode(QListView::Adjust);
    notDealMoreImageList->setViewMode(QListView::IconMode);  //设置QListWidget的显示模式
    notDealMoreImageList->setMovement(QListView::Static);    //设置QListWidget中的单元项不可被拖动
    notDealMoreImageList->setSpacing(1);                     //设置QListWidget中的单元项的间距
}


/*
* 建议：
*      维护一个checkedItems的QSet
*      添加slot接收itemChanged(QListWidgetItem *)信号
*      在其中判断checkstate()然后 checkedItems.insert() / remove()
*      这样就不用每次需要查询打钩item时都用循环来检查
*/
void FNotDealImageWidget::dealImage()
{
    QList<QList<QString> > pathGroups;
    switch (notDealImageToolBox->currentIndex())
    {
    case 0:
        if (notDealImageList->selectedItems().empty() == true)
        {
            for (int i = 0;i < notDealImageList->count(); i++)
            {
                QListWidgetItem *item = notDealImageList->item(i);
                if (item->checkState() == Qt::Checked)
                {
                    QString path = item->data(Qt::UserRole).toString();
                    pathGroups.append(QStringList(path));
                }
            }
        }
        else
        {
            for (int i = 0;i < notDealImageList->count(); i++)
            {
                QListWidgetItem *item = notDealImageList->item(i);
                if (item->isSelected () == true)
                {
                    QString path = item->data(Qt::UserRole).toString();
                    pathGroups.append(QStringList(path));
                }
            }
        }
        
        break;
    case 1:
        for (int i = 0; i < notDealMoreImageList->count(); i++)
        {
            QListWidgetItem *item = notDealMoreImageList->item(i);
            if (item->checkState() == Qt::Checked)
            {
                QStringList paths = item->data(Qt::UserRole).toStringList();
                pathGroups.append(paths);
            }
        }
        break;
    default:
        break;
    }
    emit dealImageButtonClicked(ImageType, pathGroups);
}

void FNotDealImageWidget::notDealImageItemClickSlot(QListWidgetItem *item)
{
    bool anySelected = false;
    for(int i=0;i<notDealImageList->count();i++)
    {
        if(notDealImageList->item(i)->checkState() != 0)
            anySelected = true;
    }
}

void FNotDealImageWidget::selectAll()
{
    int notDealWidgetIndex = -1;
    int isCurrentSelectedAll = 0;
    notDealWidgetIndex = notDealImageToolBox->indexOf(notDealImageToolBox->currentWidget());
    switch(notDealWidgetIndex)
    {
    case 0:
        for(int i = 0;i<notDealImageList->count();i++)
        {
            if(notDealImageList->item(i)->checkState() == 0)
            {
                notDealImageList->item(i)->setCheckState(Qt::Checked);
                notDealImageList->item(i)->setSelected(true);
            }
            else isCurrentSelectedAll++;
        }
        if (isCurrentSelectedAll == notDealImageList->count())
        {
            for(int i = 0;i<notDealImageList->count();i++)
            {
                notDealImageList->item(i)->setCheckState(Qt::Unchecked);
                notDealImageList->item(i)->setSelected(true);
            }
        }
        break;
    case 1:
        for(int i = 0;i<notDealMoreImageList->count();i++)
        {
            if(notDealMoreImageList->item(i)->checkState() == 0)
                notDealMoreImageList->item(i)->setCheckState(Qt::Checked);
            else isCurrentSelectedAll++;
        }
        if (isCurrentSelectedAll == notDealMoreImageList->count())
        {
            for(int i = 0;i<notDealMoreImageList->count();i++)
            {
                notDealMoreImageList->item(i)->setCheckState(Qt::Unchecked);
            }
        }
        break;
    default:
        break;
    }
    
}

void FNotDealImageWidget::deleteImage()
{
    int notDealWidgetIndex = -1;
    notDealWidgetIndex = notDealImageToolBox->indexOf(notDealImageToolBox->currentWidget());
    switch(notDealWidgetIndex)
    {
    case 0:
    {
        // 注：此处之前的实现用的是QListWidget的takeItem()函数，这个函数只是把QListWidgetItem从父控件中取出，并未删除，不delete的话会造成内存泄露
        QList<QListWidgetItem *> checkedItems;
        if (notDealImageList->selectedItems().empty() == true)
        {
            for (int i = 0; i < notDealImageList->count(); ++i)
            {
                QListWidgetItem *item = notDealImageList->item(i);
                if (item->checkState() == Qt::Checked)
                {
                    QString path = item->data(Qt::UserRole).toString();
                    openedImages.remove(path);
                    checkedItems << item;
                }
            }
            qDeleteAll(checkedItems);
            //				selectAllButton->setEnabled(notDealImageList->count());
        }
        else
        {
            for (int i = 0;i < notDealImageList->count(); i++)
            {
                QListWidgetItem *item = notDealImageList->item(i);
                if (item->isSelected () == true)
                {
                    QString path = item->data(Qt::UserRole).toString();
                    openedImages.remove(path);
                    checkedItems << item;
                }
            }
            qDeleteAll(checkedItems);
            //				selectAllButton->setEnabled(notDealImageList->count());
        }
        break;
    }
    case 1:
    {
        QList<QListWidgetItem *> checkedItems;
        for (int i = notDealMoreImageList->count() - 1;i >= 0;i--)
        {
            QListWidgetItem *item = notDealMoreImageList->item(i);
            if (item->checkState() == Qt::Checked)
            {
                checkedItems << item;
            }
        }
        qDeleteAll(checkedItems);
        break;
        // 疑问：这里并未删除“单张图”控件的item却删除了hash中相应的项，是否会造成bug?（再次打开组内文件时“单张图”控件中重复添加相同图片）
        //        for (int i = notDealMoreImageList->count() - 1;i >= 0;i--)
        //        {
        //            if (notDealMoreImageList->item(i)->checkState() != 0)
        //            {
        //                hash->remove(notDealMoreImageList->item(i)->data(0).toString());
        //                notDealMoreImageList->takeItem(i);
        //            }
        //        }
        //        if (notDealImageList->count() == 0)
        //        {
        //            selectAllButton->setEnabled(false);
        //        }
        //        break;
    }
    default:
        break;
    }
}

void FNotDealImageWidget::showImage()
{
    int notDealWidgetIndex = -1;
    notDealWidgetIndex = notDealImageToolBox->indexOf(notDealImageToolBox->currentWidget());
    switch(notDealWidgetIndex)
    {
    case 0:
        if (notDealImageList->selectedItems().empty() == true)
        {
            for (int i = 0; i < notDealImageList->count(); ++i)
            {
                QListWidgetItem *item = notDealImageList->item(i);
                if (item->checkState() == Qt::Checked)
                {
                    QString path = item->data(Qt::UserRole).toString();
                    emit hasImageToShow(path);
                }
            }
        }
        else
        {
            for (int i = 0;i < notDealImageList->count(); i++)
            {
                QListWidgetItem *item = notDealImageList->item(i);
                if (item->isSelected () == true)
                {
                    QString path = item->data(Qt::UserRole).toString();
                    emit hasImageToShow(path);
                }
            }
        }
        break;
    case 1:
        for (int i = 0; i < notDealMoreImageList->count(); i++)
        {
            const QListWidgetItem *item = notDealMoreImageList->item(i);
            if (item->checkState() == Qt::Checked)
            {
                QStringList paths = item->data(Qt::UserRole).toStringList();
                foreach (const QString path, paths)
                {
                    emit hasImageToShow(path);
                }
            }
        }
        break;
    default:
        break;
    }
}

void FNotDealImageWidget::onItemDoubleClicked(QListWidgetItem *item)
{
    item->setCheckState(Qt::Checked);
    QString path = item->data(Qt::UserRole).toString();
    emit hasImageToShow(path);
}

void FNotDealImageWidget::judgeCR()
{
    if(notDealImageList->selectedItems().contains(notDealImageList->currentItem())
            && notDealImageToolBox->currentWidget() == notDealImageList)
    {
        if (notDealImageList->selectedItems().count() > 1)
            picCombine();
    }
    else if( notDealMoreImageList->selectedItems().contains(notDealMoreImageList->currentItem())
             && notDealImageToolBox->currentWidget() == notDealMoreImageList)
    {
        picRank();
    }
}

void FNotDealImageWidget::picCombine()
{
    //
    picGroup = notDealImageList->selectedItems();
    if(notDealMoreImageList->count() == 0)
    {
        picSum = 0;
    }
    if(picSum == 0)
        picSum = picGroup.count();
    if(judgePicSize(picGroup))
    {
        //if(picSum == picGroup.count())
        //{
        QStringList paths;  // item.data(Qt::UserRole)中保存组内图片的地址列表
        foreach (QListWidgetItem *item, picGroup)
        {
            paths << item->data(Qt::UserRole).toString();
        }
        
        QString picGroupName = "GroupNum-" + QString::number(GroupNum++);
        QPixmap objPixmap = setIconPic(paths);
        QListWidgetItem *pItem = new QListWidgetItem(
                    QIcon(objPixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE),
                                           Qt::IgnoreAspectRatio,Qt::FastTransformation)),picGroupName);//生成QListWidgetItem对象(其Icon图像进行了伸缩[96*96])---scaled函数
        pItem->setCheckState(Qt::Unchecked);
        pItem->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE));
        pItem->setData(Qt::UserRole, paths);
        notDealMoreImageList->addItem(pItem);
        
        notDealImageToolBox->setItemIcon(1, QIcon(":/icon/new.png")); 
        //}
        ////else
        ////{
        //	QMessageBox::warning(this, tr("提示"),
        //		tr("请组合与现有分组图像相同的数量，如要添加该组，请删除现有分组"),
        //		QMessageBox::Ok);
        //}
    }
    else
    {
        QMessageBox::warning(this, tr("提示"),
                             tr("图像大小不一致"),
                             QMessageBox::Ok);
        
    }
}

bool FNotDealImageWidget::judgePicSize(QList<QListWidgetItem *> picGroup)
{
    // 获得组内首张图片的size
    QListWidgetItem *firstItem = picGroup.first();
    QString firstPath = firstItem->data(Qt::UserRole).toString();
    QSize firstSize = QImage(firstPath).size();
    // 判断剩余图片的size是否与第一张一致
    for (int i = 1; i < picGroup.size(); ++i)
    {
        const QListWidgetItem *item = picGroup[i];
        QString path = item->data(Qt::UserRole).toString();
        QSize size = QImage(path).size();
        if (size != firstSize)
            return false;
    }
    return true;
}

///制作组合后的图标
QPixmap FNotDealImageWidget::setIconPic(QList<QString> paths)
{
    QPixmap pixmap(":icon/folder.png"); //加入图标背景
    QPainter painter(&pixmap);
    
    for (int i = 0; i < 4 && i < paths.size(); ++i)
    {
        QString path = paths[i];
        QPixmap pixchild(path);
        painter.drawPixmap(55 + 200 * (i % 2), 150 + 115 * (i / 2),
                           180, 100, pixchild);
        
    }
    return pixmap;
    
}

void FNotDealImageWidget::picRank()
{
    QListWidgetItem *item = notDealMoreImageList->currentItem();
    QStringList paths = item->data(Qt::UserRole).toStringList();
    FImageRankDialog imageRankDialog(paths);
    if(imageRankDialog.exec() == QDialog::Accepted)
    {
        paths = imageRankDialog.rearrangedPaths();
        item->setData(Qt::UserRole, paths);
    }
}

void FNotDealImageWidget::toolBoxCurrentChangedSlot(int index)
{
    bool anySelected = false;
    switch(index)
    {
    case 0:
        for(int i=0;i<notDealImageList->count();i++)
        {
            if(notDealImageList->item(i)->checkState() == Qt::Checked)
                anySelected = true;
        }
        combineOrRankButton->setText(tr("组合"));
        break;
    case 1:
        for(int i=0;i<notDealMoreImageList->count();i++)
        {
            if(notDealMoreImageList->item(i)->checkState() == Qt::Checked)
                anySelected = true;
        }
        combineOrRankButton->setText(tr("排序"));
        notDealImageToolBox->setItemIcon(1, QIcon()); 
        break;
    default:
        break;
    }
}

void FNotDealImageWidget::notDealMoreImageItemClickSlot(QListWidgetItem *item)
{
    bool anySelected = false;
    for(int i=0;i<notDealMoreImageList->count();i++)
    {
        if(notDealMoreImageList->item(i)->checkState() != 0)
            anySelected = true;
    }
}

void FNotDealImageWidget::notDealMoreWidgetItemDoubleClickedSlot(QListWidgetItem *item)
{
    item->setCheckState(Qt::Checked);
    
    QStringList paths = item->data(Qt::UserRole).toStringList();
    foreach (QString path, paths)
    {
        emit hasImageToShow(path);
    }
}

void FNotDealImageWidget::setIcon(QListWidgetItem *item, const QImage &image)
{
    notDealImageList->addItem(item);
    item->setIcon(QIcon(QPixmap::fromImage(image)));
}

bool FNotDealImageWidget::alreadyOpened(QString path)
{
    foreach (QString openedPath, openedImages.values())
    {
        if (!QString::compare(path, openedPath, Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}
