<?php
function decode(&$data){
    $decoded = new stdClass();
    $table = getRevTable();
  /* Alloc 1 */
    $validTable = getValidTable();
  /* Alloc 3 */
    $padding = 0.0;
  if(count($data) > 1.0){
    if($data[count($data) - 1.0] == "="){
            $padding = $padding + 1.0;
    }
  }
  /* TODO: Evaluate && left to right and stop if false? */
  if(count($data) > 2.0){
    if($data[count($data) - 2.0] == "="){
            $padding = $padding + 1.0;
    }
  }
    $quads = count($data)/4.0;

  /* TODO: Require init? */
  /* Init */
    $decoded->data = array_fill(0, $quads*3.0 - $padding, 0);
    $decoded->errorMessage = array_fill(0, 0.0, 0);
    $decoded->success = true;

    $done = false;
  for($i = 0.0; $i < $quads &&  !$done ; $i = $i + 1.0){
        $data4 = array_fill(0, 4.0, 0);
        $currentQuad = $i*4.0;
        $currentTriplet = $i*3.0;
    if($padding > 0.0 && ($i + 1.0) == $quads){
      if($padding == 2.0){
                $data4[0.0] = $data[$currentQuad + 0.0];
                $data4[1.0] = $data[$currentQuad + 1.0];
                $data4[2.0] = "A";
                $data4[3.0] = "A";
      }else if($padding == 1.0){
                $data4[0.0] = $data[$currentQuad + 0.0];
                $data4[1.0] = $data[$currentQuad + 1.0];
                $data4[2.0] = $data[$currentQuad + 2.0];
                $data4[3.0] = "A";
      }

    }else{
            $data4[0.0] = $data[$currentQuad + 0.0];
            $data4[1.0] = $data[$currentQuad + 1.0];
            $data4[2.0] = $data[$currentQuad + 2.0];
            $data4[3.0] = $data[$currentQuad + 3.0];
    }
        $decoded3 = decode3table($data4, $table, $validTable);
    if($decoded3->success){
      if($padding > 0.0 && $i + 1.0 == $quads){
        if($padding == 2.0){
                    $decoded->data[$currentTriplet + 0.0] = $decoded3->data[0.0];
        }else if($padding == 1.0){
                    $decoded->data[$currentTriplet + 0.0] = $decoded3->data[0.0];
                    $decoded->data[$currentTriplet + 1.0] = $decoded3->data[1.0];
        }

      }else{
                $decoded->data[$currentTriplet + 0.0] = $decoded3->data[0.0];
                $decoded->data[$currentTriplet + 1.0] = $decoded3->data[1.0];
                $decoded->data[$currentTriplet + 2.0] = $decoded3->data[2.0];
      }
    }else{
            $decoded->success = false;
            $decoded->errorMessage = $decoded3->errorMessage;
            $done = true;
    }
    unset($decoded3);
  }

  unset($table);
  /* Unalloc 1 */
  return $decoded;
}
function decode3(&$data){
    $table = getRevTable();
  /* Alloc 1 */
    $validTable = getValidTable();
  /* Alloc 2 */
    $decoded = decode3table($data, $table, $validTable);

  unset($validTable);
  /* Unalloc 2 */
  unset($table);
  /* Unalloc 1 */
  return $decoded;
}
function decode3table(&$data, &$table, &$validTable){
    $decoded = new stdClass();
  /* Alloc r */
    $decoded->success = false;
    $decoded->data = array_fill(0, 3.0, 0);
    $decoded->errorMessage = array();

  if(count($data) == 4.0){
        $valid = 0.0;
    for($i = 0.0; $i < 4.0; $i = $i + 1.0){
      if(isValidBase64characterTable($data[$i], $validTable)){
                $valid = $valid + 1.0;
      }
    }
    if($valid == 4.0){
      decode3NoChecksTable($decoded, $data, $table);
            $decoded->success = true;
    }else{
            $decoded->errorMessage = str_split("Invalid character in input string.");
            $decoded->success = false;
    }
  }else{
        $decoded->errorMessage = str_split("There must be exactly four characters in the input string.");
        $decoded->success = false;
  }

  return $decoded;
}
function isValidBase64characterTable($c, &$validTable){
  return $validTable[ord($c)];
}
function decode3NoChecks($decoded, &$data){
    $table = getRevTable();
  decode3NoChecksTable($decoded, $data, $table);
}
function decode3NoChecksTable($decoded, &$data, &$table){
    $total = 0.0;
  for($i = 0.0; $i < 4.0; $i = $i + 1.0){
        $n = getNumber($data[4.0 - $i - 1.0], $table);
        $total = $total + $n*pow(2.0, $i*6.0);
  }

  for($i = 0.0; $i < 3.0; $i = $i + 1.0){
        $r = $total%pow(2.0, 8.0);
        $decoded->data[3.0 - $i - 1.0] = $r;
        $total = ($total - $r)/pow(2.0, 8.0);
  }
}
function getNumber($c, &$table){
  return $table[ord($c)];
}
function encode(&$data){
    $encoded = new stdClass();

    $padding = 0.0;
  if((count($data)%3.0) == 1.0){
        $padding = 2.0;
  }
  if((count($data)%3.0) == 2.0){
        $padding = 1.0;
  }
    $triplets = ceil(count($data)/3.0);

  /* Init */
    $encoded->data = array_fill(0, $triplets*4.0, 0);
    $encoded->errorMessage = array_fill(0, 0.0, 0);
    $encoded->success = true;

    $done = false;
  for($i = 0.0; $i < $triplets &&  !$done ; $i = $i + 1.0){
        $data3 = array_fill(0, 3.0, 0);
        $currentTriplet = $i*3.0;
        $currentQuad = $i*4.0;
    if($padding > 0.0 && $i + 1.0 == $triplets){
      if($padding == 2.0){
                $data3[0.0] = $data[$currentTriplet + 0.0];
                $data3[1.0] = 0.0;
                $data3[2.0] = 0.0;
      }else if($padding == 1.0){
                $data3[0.0] = $data[$currentTriplet + 0.0];
                $data3[1.0] = $data[$currentTriplet + 1.0];
                $data3[2.0] = 0.0;
      }

    }else{
            $data3[0.0] = $data[$currentTriplet + 0.0];
            $data3[1.0] = $data[$currentTriplet + 1.0];
            $data3[2.0] = $data[$currentTriplet + 2.0];
    }
        $encoded3 = encode3($data3);
    if($encoded3->success){
            $encoded->data[$currentQuad + 0.0] = $encoded3->data[0.0];
            $encoded->data[$currentQuad + 1.0] = $encoded3->data[1.0];
      if($padding > 0.0 && $i + 1.0 == $triplets){
        if($padding == 2.0){
                    $encoded->data[$currentQuad + 2.0] = "=";
                    $encoded->data[$currentQuad + 3.0] = "=";
        }else if($padding == 1.0){
                    $encoded->data[$currentQuad + 2.0] = $encoded3->data[2.0];
                    $encoded->data[$currentQuad + 3.0] = "=";
        }

      }else{
                $encoded->data[$currentQuad + 2.0] = $encoded3->data[2.0];
                $encoded->data[$currentQuad + 3.0] = $encoded3->data[3.0];
      }
    }else{
            $encoded->success = false;
            $encoded->errorMessage = $encoded3->errorMessage;
            $done = true;
    }
    unset($encoded3);
  }

  return $encoded;
}
function encode3(&$data){
    $encoded = new stdClass();
  /* Init */
    $encoded->data = array_fill(0, 4.0, 0);
    $encoded->data[0.0] = "A";
    $encoded->data[1.0] = "A";
    $encoded->data[2.0] = "A";
    $encoded->data[3.0] = "A";
    $encoded->errorMessage = array_fill(0, 0.0, 0);
    $encoded->success = false;

  /* Check. */
  if(count($data) == 3.0){
        $elementsVerified = 0.0;
    for($i = 0.0; $i < count($data); $i = $i + 1.0){
            $e = $data[$i];
            $isWithinBounds = ($e >= 0.0) && ($e < pow(2.0, 8.0));
            $isWhole = ($e%1.0) == 0.0;
      if($isWithinBounds && $isWhole){
                $elementsVerified = $elementsVerified + 1.0;
      }else{
                $encoded->errorMessage = str_split("Input number is too high, too low or is not a whole number.");
                $encoded->success = false;
      }
    }
    if($elementsVerified == count($data)){
      encode3NoChecks($encoded, $data);
            $encoded->success = true;
    }
  }else{
        $encoded->errorMessage = str_split("Input must contain 3 numbers.");
        $encoded->success = false;
  }

  return $encoded;
}
function encode3NoChecks($encoded, &$data){
    $total = 0.0;
  for($i = 0.0; $i < count($data); $i = $i + 1.0){
        $total = $total + $data[count($data) - $i - 1.0]*pow(2.0, $i*8.0);
  }

  for($i = 0.0; $i < 4.0; $i = $i + 1.0){
        $bit6 = $total%pow(2.0, 6.0);
        $total = ($total - $bit6)/pow(2.0, 6.0);
        $c = getCharacter($bit6);
        $encoded->data[4.0 - $i - 1.0] = $c;
  }
}
function getCharacter($bit6){
    $table = getTable();

  return $table[$bit6];
}
function &getTable(){
  return str_split("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
}
function &getRevTable(){
    $table = getTable();
    $max = getMax($table) + 1.0;
    $revTable = array_fill(0, $max + 1.0, 0);

  for($i = 0.0; $i < count($table); $i = $i + 1.0){
        $revTable[ord($table[$i])] = $i;
  }

  return $revTable;
}
function &getValidTable(){
    $table = getTable();
    $max = getMax($table) + 1.0;
    $validTable = array_fill(0, $max, 0);

  for($i = 0.0; $i < $max; $i = $i + 1.0){
        $validTable[$i] = isValidBase64character(chr($i));
  }

  return $validTable;
}
function getMax(&$table){
    $maxValue = 0.0;

  for($i = 0.0; $i < count($table); $i = $i + 1.0){
        $v = ord($table[$i]);
        $maxValue = max($maxValue, $v);
  }
  return $maxValue;
}
function isValidBase64character($c){
    $table = getTable();
    $isValid = false;

  for($i = 0.0; $i < count($table); $i = $i + 1.0){
    if($table[$i] == $c){
            $isValid = true;
    }
  }

  return $isValid;
}
function testDecoder(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

    $orginal = str_split("Hei");
    $data = stringToNumberArray($orginal);
    $encoded = encode($data);

    $decoded = decode3($encoded->data);

    $result = numberArrayToString($decoded->data);

  if( !stringsEqual($orginal, $result) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$decoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($decoded->errorMessage);
  }

  return $atestresult;
}
function testDecoder2(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

    $original = array_fill(0, 3.0, 0);
    $original[0.0] = 0.0;
    $original[1.0] = pow(2.0, 4.0);
    $original[2.0] = pow(2.0, 8.0) - 1.0;
    $encoded = encode($original);

    $decoded = decode3($encoded->data);

  if( !numberArraysEqual($original, $decoded->data) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$decoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($decoded->errorMessage);
  }

  return $atestresult;
}
function testdecoderWithPadding(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

    $original = str_split("Hei12");
    $data = stringToNumberArray($original);
    $encoded = encode($data);

    $decoded = decode($encoded->data);

    $result = numberArrayToString($decoded->data);

  if( !stringsEqual($original, $result) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$decoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($decoded->errorMessage);
  }

  return $atestresult;
}
function testDecoderWithPadding2(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

    $original = str_split("Hei1");
    $data = stringToNumberArray($original);
    $encoded = encode($data);

    $decoded = decode($encoded->data);

    $result = numberArrayToString($decoded->data);

  if( !stringsEqual($original, $result) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$decoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($decoded->errorMessage);
  }

  return $atestresult;
}
function testDecoderReal1(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

    $decoded = decode(str_split("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g="));

  if( !$decoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($decoded->errorMessage);
  }

  return $atestresult;
}
function testEncoder(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 0.0, 0);

    $hei = str_split("Hei");

    $data = stringToNumberArray($hei);

    $encoded = encode3($data);

  if( !stringsEqual($encoded->data, str_split("SGVp")) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$encoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($encoded->errorMessage);
  }

  return $atestresult;
}
function stringFrom(&$src){
    $str = new stdClass();
    $str->str = $src;
  return $str;
}
function testEncoder2(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

  /* TODO: Support direct instantiation. */
    $data = array_fill(0, 3.0, 0);
    $data[0.0] = 0.0;
    $data[1.0] = pow(2.0, 4.0);
    $data[2.0] = pow(2.0, 8.0) - 1.0;

    $encoded = encode3($data);

  if( !stringsEqual($encoded->data, str_split("ABD/")) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$encoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($encoded->errorMessage);
  }

  return $atestresult;
}
function testEncoderWithPadding(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

    $hei = str_split("Hei12");

    $data = stringToNumberArray($hei);

    $encoded = encode($data);

  if( !stringsEqual($encoded->data, str_split("SGVpMTI=")) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$encoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($encoded->errorMessage);
  }

  return $atestresult;
}
function testEncoderWithPadding2(){
    $atestresult = new stdClass();
    $atestresult->failures = 0.0;
    $atestresult->errorMessages = array_fill(0, 1.0, 0);

    $hei = str_split("Hei1");

    $data = stringToNumberArray($hei);

    $encoded = encode($data);

  if( !stringsEqual($encoded->data, str_split("SGVpMQ==")) ){
        $atestresult->failures = $atestresult->failures + 1.0;
  }

  if( !$encoded->success ){
        $atestresult->failures = $atestresult->failures + 1.0;
        $atestresult->errorMessages[0.0] = stringFrom($encoded->errorMessage);
  }

  return $atestresult;
}
function test(){
    $failures = 0.0;

    $atestresult = testDecoder();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testDecoder2();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testdecoderWithPadding();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testDecoderWithPadding2();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testEncoder();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testEncoder2();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testEncoderWithPadding();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testEncoderWithPadding2();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

    $atestresult = testDecoderReal1();
  if($atestresult->failures != 0.0){
        $failures = $failures + 1.0;
  }

  return $failures;
}
function &stringToNumberArray(&$string){
    $array = array_fill(0, count($string), 0);

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
        $array[$i] = ord($string[$i]);
  }
  return $array;
}
function &numberArrayToString(&$array){
    $string = array_fill(0, count($array), 0);

  for($i = 0.0; $i < count($array); $i = $i + 1.0){
        $string[$i] = chr($array[$i]);
  }
  return $string;
}
function stringsEqual(&$data1, &$data2){
    $equal = false;
  if(count($data1) == count($data2)){
        $nrEqual = 0.0;
    for($i = 0.0; $i < count($data1); $i = $i + 1.0){
      if($data1[$i] == $data2[$i]){
                $nrEqual = $nrEqual + 1.0;
      }
    }
    if($nrEqual == count($data1)){
            $equal = true;
    }
  }else{
        $equal = false;
  }

  return $equal;
}
function numberArraysEqual(&$data1, &$data2){
    $equal = false;
  if(count($data1) == count($data2)){
        $nrEqual = 0.0;
    for($i = 0.0; $i < count($data1); $i = $i + 1.0){
      if($data1[$i] == $data2[$i]){
                $nrEqual = $nrEqual + 1.0;
      }
    }
    if($nrEqual == count($data1)){
            $equal = true;
    }
  }else{
        $equal = false;
  }

  return $equal;
}


echo test();