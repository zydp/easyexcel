# easyexcel
A PHP extension based on libxl, read-write .xls & .xlsx

# build
    git clone http://github.com/zydp/easyexcel.git
    
    cd easyexcel
    
    phpize
  
    ./configure  //can add the php inlcude dir use ./configure --help to get it
  
    make
  
    make install
    
    add the "extension=easyexcel.so" to your php setup file php.ini
    
    php -d extension=modules/easyexcel.so --re easyexcel  //Check to be sure it loads
    
    use php -m to view the loaded module, make sure you find the module easyexcel

# using example
# read
     $e = new easyexcel("xlsx");
     $e->LoadFile("example.xlsx");
    
    /*
      //traversal xls
      $e = new easyexcel("xls");
      $e->LoadFile("example2.xls"); 
    */
    
    $sheet_count = $e->SheetCount();
    echo $sheet_count.PHP_EOL;

    for($s = 0; $s < $sheet_count; $s++) {
      $e->SheetSwitch($s);
      $row = $e->GetTotalRow();
      $col = $e->GetTotalCol();
      for($r = 0; $r < $row; $r++){
        for($c = 0; $c < $col; $c++){
          switch($e->GetCellType($r, $c))
          {
            //case 0:   //TYPE_EMPTY
            case 1:     //TYPE_NUMBER
              print($e->ReadNum($r, $c)." ");
            break;
            case 2:     //TYPE_STRING
              print($e->ReadStr($r, $c)." ");
            break;
            //case 3:       //TYPE_BOOLEAN
            //break;
            case 4:     //TYPE_BLANK
              print(" - ");
            break;
            //case 5:   //TYPE_ERROR
            //break;
          }
        }
        echo PHP_EOL;
      }
    }

# write
    $e2 = new easyexcel("xlsx");
    $e2->AddSheetAndSwitchTo("tsheet");
    for($r = 0; $r < 100000; $r++){
        for($c = 0; $c < 10; $c++){
            $e2->WriteStr($r, $c, (string)$r."-".$c);
            //echo (string)$r."-".$c.PHP_EOL;
        }
    }
    $e2->SaveToFile("example3.xlsx");
    
# API list
    LoadFile               //加载一个文件                      参数：文件名                                 返回：bool
    SheetCount             //返回sheet总数                    参数：无                                      返回：int
    SheetSwitch            //切换sheet                        参数：sheet下标                              返回：bool
    AddSheetAndSwitchTo    //添加一张新sheet，并切换到该sheet   参数：sheet名                                返回：bool
    GetSheetNameByIndex    //跟据序号获取sheet名，序号从0开始   参数：sheet下标                               返回：str
    SetSheetNameByIndex    //跟据序号设置sheet名，序号从0开始   参数：sheet下标，sheet新名                   返回：bool
    GetTotalRow            //获取当前sheet的总行数             参数：无                                     返回：int
    GetTotalCol            //获取当前sheet总列数               参数：无                                    返回：int
    GetCellType            //获取单元格数据类型                参数：row, col                               返回：CellType
    ReadNum                //读取数字                         参数：row, col                               返回：int
    ReadStr                //读取字符串                       参数：row, col                                返回：str
    WriteNum               //写入数字                         参数：row, col, num                          返回：bool
    WriteStr               //写入字符串                       参数：row, col, str                           返回：bool
    IsFormula              //该单元格是否是公式                参数：row, col                               返回：bool
    ReadFormula            //读取公式                         参数：row, col                               返回：str
    WriteFormula           //写入公式                         参数：row, col, str                          返回：bool
    SaveToFile             //保存到文件                       参数：s文件名                                返回：bool
    SetMerge              //合并单元格                        参数：rowFirst, rowLast, colFirst, colLast    返回：bool
    DelMerge              //取消已合并的单元格                 参数：row, col                               返回：bool
    InsertRow             //插入行                            参数：rowFirst, rowLast                       返回：bool
    DelRow                //删除行                            参数：rowFirst, rowLast                       返回：bool
    InsertCol             //插入列                            参数：colFirst, colLast                       返回：bool
    DelCol                //删除列                            参数：colFirst, colLast                       返回：bool
    CopyCell              //复制单元格                        参数：rowSrc, colSrc, rowDst, colDst          返回：bool
    ClearCell             //清空单元格                        参数：rowFirst, rowLast, colFirst, colLast    返回：bool
    SetRowHeigh           //设置行高                          参数：row, heigh                               返回：bool
    SetColWidth           //设置列宽                          参数：colFirst, colLast, width                 返回：bool
    RowHide               //隐藏行                            参数：row, hide->BOOL                           返回：bool
    ColHide               //隐藏列                            参数：col, hide->BOOL                           返回：bool
    
# CellType
    typedef enum{
        CELLTYPE_EMPTY,        //0
        CELLTYPE_NUMBER,       //1
        CELLTYPE_STRING,       //2
        CELLTYPE_BOOLEAN,      //3
        CELLTYPE_BLANK,        //4
        CELLTYPE_ERROR         //5
    }CellType;
