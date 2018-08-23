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

# use example
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
            //case 0:	//TYPE_EMPTY
            case 1:		//TYPE_NUMBER
              print($e->ReadNum($r, $c)." ");
            break;
            case 2:		//TYPE_STRING
              print($e->ReadStr($r, $c)." ");
            break;
            //case 3:		//TYPE_BOOLEAN
            //break;
            case 4:		//TYPE_BLANK
              print(" - ");
            break;
            //case 5:	//TYPE_ERROR
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
