function decode(data){
  var done;
  var data4;
  var quads, i, currentQuad, currentTriplet, padding;
  var decoded3, decoded;
  var table;
  var validTable;
  decoded = {};
  table = getRevTable();
  /* Alloc 1 */
  validTable = getValidTable();
  /* Alloc 3 */
  padding = 0;
  if(data.length > 1){
    if(data[data.length - 1] == '='){
      padding = padding + 1;
    }
  }
  /* TODO: Evaluate && left to right and stop if false? */
  if(data.length > 2){
    if(data[data.length - 2] == '='){
      padding = padding + 1;
    }
  }
  quads = data.length/4;

  /* TODO: Require init? */
  /* Init */
  decoded.data = [];
  decoded.data.length = quads*3 - padding;
  decoded.errorMessage = [];
  decoded.errorMessage.length = 0;
  decoded.success = true;

  done = false;
  for(i = 0; i < quads &&  !done ; i = i + 1){
    data4 = [];
    data4.length = 4;
    currentQuad = i*4;
    currentTriplet = i*3;
    if(padding > 0 && (i + 1) == quads){
      if(padding == 2){
        data4[0] = data[currentQuad + 0];
        data4[1] = data[currentQuad + 1];
        data4[2] = 'A';
        data4[3] = 'A';
      }else if(padding == 1){
        data4[0] = data[currentQuad + 0];
        data4[1] = data[currentQuad + 1];
        data4[2] = data[currentQuad + 2];
        data4[3] = 'A';
      }

    }else{
      data4[0] = data[currentQuad + 0];
      data4[1] = data[currentQuad + 1];
      data4[2] = data[currentQuad + 2];
      data4[3] = data[currentQuad + 3];
    }
    decoded3 = decode3table(data4, table, validTable);
    if(decoded3.success){
      if(padding > 0 && i + 1 == quads){
        if(padding == 2){
          decoded.data[currentTriplet + 0] = decoded3.data[0];
        }else if(padding == 1){
          decoded.data[currentTriplet + 0] = decoded3.data[0];
          decoded.data[currentTriplet + 1] = decoded3.data[1];
        }

      }else{
        decoded.data[currentTriplet + 0] = decoded3.data[0];
        decoded.data[currentTriplet + 1] = decoded3.data[1];
        decoded.data[currentTriplet + 2] = decoded3.data[2];
      }
    }else{
      decoded.success = false;
      decoded.errorMessage = decoded3.errorMessage;
      done = true;
    }
    delete(decoded3);
  }

  delete(table);
  /* Unalloc 1 */
  return decoded;
}
function decode3(data){
  var table;
  var validTable;
  var decoded;
  table = getRevTable();
  /* Alloc 1 */
  validTable = getValidTable();
  /* Alloc 2 */
  decoded = decode3table(data, table, validTable);

  delete(validTable);
  /* Unalloc 2 */
  delete(table);
  /* Unalloc 1 */
  return decoded;
}
function decode3table(data, table, validTable){
  var i, valid;
  var decoded;
  decoded = {};
  /* Alloc r */
  decoded.success = false;
  decoded.data = [];
  decoded.data.length = 3;
  decoded.errorMessage = "";

  if(data.length == 4){
    valid = 0;
    for(i = 0; i < 4; i = i + 1){
      if(isValidBase64characterTable(data[i], validTable)){
        valid = valid + 1;
      }
    }
    if(valid == 4){
      decode3NoChecksTable(decoded, data, table);
      decoded.success = true;
    }else{
      decoded.errorMessage = "Invalid character in input string.";
      decoded.success = false;
    }
  }else{
    decoded.errorMessage = "There must be exactly four characters in the input string.";
    decoded.success = false;
  }

  return decoded;
}
function isValidBase64characterTable(c, validTable){
  return validTable[c.codePointAt(0)];
}
function decode3NoChecks(decoded, data){
  var table;
  table = getRevTable();
  decode3NoChecksTable(decoded, data, table);
}
function decode3NoChecksTable(decoded, data, table){
  var total, i, n, r;
  total = 0;
  for(i = 0; i < 4; i = i + 1){
    n = getNumber(data[4 - i - 1], table);
    total = total + n*Math.pow(2, i*6);
  }

  for(i = 0; i < 3; i = i + 1){
    r = total%Math.pow(2, 8);
    decoded.data[3 - i - 1] = r;
    total = (total - r)/Math.pow(2, 8);
  }
}
function getNumber(c, table){
  return table[c.codePointAt(0)];
}
function encode(data){
  var encoded, encoded3;
  var padding, triplets, i, currentTriplet, currentQuad;
  var data3;
  var done;
  encoded = {};

  padding = 0;
  if((data.length%3) == 1){
    padding = 2;
  }
  if((data.length%3) == 2){
    padding = 1;
  }
  triplets = Math.ceil(data.length/3);

  /* Init */
  encoded.data = [];
  encoded.data.length = triplets*4;
  encoded.errorMessage = [];
  encoded.errorMessage.length = 0;
  encoded.success = true;

  done = false;
  for(i = 0; i < triplets &&  !done ; i = i + 1){
    data3 = [];
    data3.length = 3;
    currentTriplet = i*3;
    currentQuad = i*4;
    if(padding > 0 && i + 1 == triplets){
      if(padding == 2){
        data3[0] = data[currentTriplet + 0];
        data3[1] = 0;
        data3[2] = 0;
      }else if(padding == 1){
        data3[0] = data[currentTriplet + 0];
        data3[1] = data[currentTriplet + 1];
        data3[2] = 0;
      }

    }else{
      data3[0] = data[currentTriplet + 0];
      data3[1] = data[currentTriplet + 1];
      data3[2] = data[currentTriplet + 2];
    }
    encoded3 = encode3(data3);
    if(encoded3.success){
      encoded.data[currentQuad + 0] = encoded3.data[0];
      encoded.data[currentQuad + 1] = encoded3.data[1];
      if(padding > 0 && i + 1 == triplets){
        if(padding == 2){
          encoded.data[currentQuad + 2] = '=';
          encoded.data[currentQuad + 3] = '=';
        }else if(padding == 1){
          encoded.data[currentQuad + 2] = encoded3.data[2];
          encoded.data[currentQuad + 3] = '=';
        }

      }else{
        encoded.data[currentQuad + 2] = encoded3.data[2];
        encoded.data[currentQuad + 3] = encoded3.data[3];
      }
    }else{
      encoded.success = false;
      encoded.errorMessage = encoded3.errorMessage;
      done = true;
    }
    delete(encoded3);
  }

  return encoded;
}
function encode3(data){
  var encoded;
  var elementsVerified, i, e;
  var isWithinBounds, isWhole;
  encoded = {};
  /* Init */
  encoded.data = [];
  encoded.data.length = 4;
  encoded.data[0] = 'A';
  encoded.data[1] = 'A';
  encoded.data[2] = 'A';
  encoded.data[3] = 'A';
  encoded.errorMessage = [];
  encoded.errorMessage.length = 0;
  encoded.success = false;

  /* Check. */
  if(data.length == 3){
    elementsVerified = 0;
    for(i = 0; i < data.length; i = i + 1){
      e = data[i];
      isWithinBounds = (e >= 0) && (e < Math.pow(2, 8));
      isWhole = (e%1) == 0;
      if(isWithinBounds && isWhole){
        elementsVerified = elementsVerified + 1;
      }else{
        encoded.errorMessage = "Input number is too high, too low or is not a whole number.";
        encoded.success = false;
      }
    }
    if(elementsVerified == data.length){
      encode3NoChecks(encoded, data);
      encoded.success = true;
    }
  }else{
    encoded.errorMessage = "Input must contain 3 numbers.";
    encoded.success = false;
  }

  return encoded;
}
function encode3NoChecks(encoded, data){
  var total, i, bit6;
  var c;
  total = 0;
  for(i = 0; i < data.length; i = i + 1){
    total = total + data[data.length - i - 1]*Math.pow(2, i*8);
  }

  for(i = 0; i < 4; i = i + 1){
    bit6 = total%Math.pow(2, 6);
    total = (total - bit6)/Math.pow(2, 6);
    c = getCharacter(bit6);
    encoded.data[4 - i - 1] = c;
  }
}
function getCharacter(bit6){
  var table;
  table = getTable();

  return table[bit6];
}
function getTable(){
  return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}
function getRevTable(){
  var table;
  var i, max;
  var revTable;
  table = getTable();
  max = getMax(table) + 1;
  revTable = [];
  revTable.length = max + 1;

  for(i = 0; i < table.length; i = i + 1){
    revTable[table[i].codePointAt(0)] = i;
  }

  return revTable;
}
function getValidTable(){
  var table;
  var max, i;
  var validTable;
  table = getTable();
  max = getMax(table) + 1;
  validTable = [];
  validTable.length = max;

  for(i = 0; i < max; i = i + 1){
    validTable[i] = isValidBase64character(String.fromCharCode(i));
  }

  return validTable;
}
function getMax(table){
  var maxValue;
  var v;
  var i;
  maxValue = 0;

  for(i = 0; i < table.length; i = i + 1){
    v = table[i].codePointAt(0);
    maxValue = Math.max(maxValue, v);
  }
  return maxValue;
}
function isValidBase64character(c){
  var table;
  var isValid;
  var i;
  table = getTable();
  isValid = false;

  for(i = 0; i < table.length; i = i + 1){
    if(table[i] == c){
      isValid = true;
    }
  }

  return isValid;
}
function testDecoder(){
  var orginal, result;
  var data;
  var encoded;
  var decoded;
  var atestresult;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  orginal = "Hei";
  data = stringToNumberArray(orginal);
  encoded = encode(data);

  decoded = decode3(encoded.data);

  result = numberArrayToString(decoded.data);

  if( !stringsEqual(orginal, result) ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !decoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
  }

  return atestresult;
}
function testDecoder2(){
  var atestresult;
  var original;
  var encoded;
  var decoded;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  original = [];
  original.length = 3;
  original[0] = 0;
  original[1] = Math.pow(2, 4);
  original[2] = Math.pow(2, 8) - 1;
  encoded = encode(original);

  decoded = decode3(encoded.data);

  if( !numberArraysEqual(original, decoded.data) ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !decoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
  }

  return atestresult;
}
function testdecoderWithPadding(){
  var atestresult;
  var original;
  var data;
  var encoded;
  var decoded;
  var result;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  original = "Hei12";
  data = stringToNumberArray(original);
  encoded = encode(data);

  decoded = decode(encoded.data);

  result = numberArrayToString(decoded.data);

  if( !stringsEqual(original, result) ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !decoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
  }

  return atestresult;
}
function testDecoderWithPadding2(){
  var atestresult;
  var original;
  var data;
  var encoded;
  var decoded;
  var result;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  original = "Hei1";
  data = stringToNumberArray(original);
  encoded = encode(data);

  decoded = decode(encoded.data);

  result = numberArrayToString(decoded.data);

  if( !stringsEqual(original, result) ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !decoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
  }

  return atestresult;
}
function testDecoderReal1(){
  var atestresult;
  var decoded;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  decoded = decode("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g=");

  if( !decoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
  }

  return atestresult;
}
function testEncoder(){
  var atestresult;
  var hei;
  var data;
  var encoded;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 0;

  hei = "Hei";

  data = stringToNumberArray(hei);

  encoded = encode3(data);

  if( !stringsEqual(encoded.data, "SGVp") ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !encoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
  }

  return atestresult;
}
function stringFrom(src){
  var str;
  str = {};
  str.str = src;
  return str;
}
function testEncoder2(){
  var atestresult;
  var data;
  var encoded;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  /* TODO: Support direct instantiation. */
  data = [];
  data.length = 3;
  data[0] = 0;
  data[1] = Math.pow(2, 4);
  data[2] = Math.pow(2, 8) - 1;

  encoded = encode3(data);

  if( !stringsEqual(encoded.data, "ABD/") ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !encoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
  }

  return atestresult;
}
function testEncoderWithPadding(){
  var atestresult;
  var hei;
  var data;
  var encoded;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  hei = "Hei12";

  data = stringToNumberArray(hei);

  encoded = encode(data);

  if( !stringsEqual(encoded.data, "SGVpMTI=") ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !encoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
  }

  return atestresult;
}
function testEncoderWithPadding2(){
  var atestresult;
  var hei;
  var data;
  var encoded;
  atestresult = {};
  atestresult.failures = 0;
  atestresult.errorMessages = [];
  atestresult.errorMessages.length = 1;

  hei = "Hei1";

  data = stringToNumberArray(hei);

  encoded = encode(data);

  if( !stringsEqual(encoded.data, "SGVpMQ==") ){
    atestresult.failures = atestresult.failures + 1;
  }

  if( !encoded.success ){
    atestresult.failures = atestresult.failures + 1;
    atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
  }

  return atestresult;
}
function test(){
  var failures;
  var atestresult;
  failures = 0;

  atestresult = testDecoder();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testDecoder2();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testdecoderWithPadding();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testDecoderWithPadding2();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testEncoder();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testEncoder2();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testEncoderWithPadding();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testEncoderWithPadding2();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  atestresult = testDecoderReal1();
  if(atestresult.failures != 0){
    failures = failures + 1;
  }

  return failures;
}
function stringToNumberArray(string){
  var i;
  var array;
  array = [];
  array.length = string.length;

  for(i = 0; i < string.length; i = i + 1){
    array[i] = string[i].codePointAt(0);
  }
  return array;
}
function numberArrayToString(array){
  var i;
  var string;
  string = [];
  string.length = array.length;

  for(i = 0; i < array.length; i = i + 1){
    string[i] = String.fromCharCode(array[i]);
  }
  return string;
}
function stringsEqual(data1, data2){
  var equal;
  var nrEqual, i;
  equal = false;
  if(data1.length == data2.length){
    nrEqual = 0;
    for(i = 0; i < data1.length; i = i + 1){
      if(data1[i] == data2[i]){
        nrEqual = nrEqual + 1;
      }
    }
    if(nrEqual == data1.length){
      equal = true;
    }
  }else{
    equal = false;
  }

  return equal;
}
function numberArraysEqual(data1, data2){
  var equal;
  var nrEqual, i;
  equal = false;
  if(data1.length == data2.length){
    nrEqual = 0;
    for(i = 0; i < data1.length; i = i + 1){
      if(data1[i] == data2[i]){
        nrEqual = nrEqual + 1;
      }
    }
    if(nrEqual == data1.length){
      equal = true;
    }
  }else{
    equal = false;
  }

  return equal;
}


console.log(test());