#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define strparam(str) (str), strlen(str)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Decoded;
typedef struct Decoded Decoded;

struct Encoded;
typedef struct Encoded Encoded;

struct string;
typedef struct string string;

struct testresult;
typedef struct testresult testresult;

struct Decoded{
  double *data;
  size_t dataLength;
  _Bool success;
  char *errorMessage;
  size_t errorMessageLength;
};

struct Encoded{
  char *data;
  size_t dataLength;
  _Bool success;
  char *errorMessage;
  size_t errorMessageLength;
};

struct string{
  char *str;
  size_t strLength;
};

struct testresult{
  double failures;
  string **errorMessages;
  size_t errorMessagesLength;
};


Decoded *decode(char *data, size_t dataLength);
Decoded *decode3(char *data, size_t dataLength);
Decoded *decode3table(char *data, size_t dataLength, double *table, size_t tableLength, _Bool *validTable, size_t validTableLength);
_Bool isValidBase64characterTable(char c, _Bool *validTable, size_t validTableLength);
void decode3NoChecks(Decoded *decoded, char *data, size_t dataLength);
void decode3NoChecksTable(Decoded *decoded, char *data, size_t dataLength, double *table, size_t tableLength);
double getNumber(char c, double *table, size_t tableLength);

Encoded *encode(double *data, size_t dataLength);
Encoded *encode3(double *data, size_t dataLength);
void encode3NoChecks(Encoded *encoded, double *data, size_t dataLength);
char getCharacter(double bit6);

char *getTable(size_t *returnArrayLength);
double *getRevTable(size_t *returnArrayLength);
_Bool *getValidTable(size_t *returnArrayLength);
double getMax(char *table, size_t tableLength);
_Bool isValidBase64character(char c);

testresult *testDecoder();
testresult *testDecoder2();
testresult *testdecoderWithPadding();
testresult *testDecoderWithPadding2();
testresult *testDecoderReal1();

testresult *testEncoder();
string *stringFrom(char *src, size_t srcLength);
testresult *testEncoder2();
testresult *testEncoderWithPadding();
testresult *testEncoderWithPadding2();

double test();

double *stringToNumberArray(size_t *returnArrayLength, char *string, size_t stringLength);
char *numberArrayToString(size_t *returnArrayLength, double *array, size_t arrayLength);
_Bool stringsEqual(char *data1, size_t data1Length, char *data2, size_t data2Length);
_Bool numberArraysEqual(double *data1, size_t data1Length, double *data2, size_t data2Length);

Decoded *decode(char *data, size_t dataLength){
  _Bool done;
  char *data4;
  size_t data4Length;
  double quads, i, currentQuad, currentTriplet, padding;
  Decoded *decoded3, *decoded;
  double *table;
  size_t tableLength;
  _Bool *validTable;
  size_t validTableLength;

  decoded = (Decoded *)malloc(sizeof(Decoded));
  table = getRevTable(&tableLength);
  /* Alloc 1 */
  validTable = getValidTable(&validTableLength);
  /* Alloc 3 */
  padding = 0.0;
  if(dataLength > 1.0){
    if(data[(int)(dataLength - 1.0)] == '='){
      padding = padding + 1.0;
    }
  }
  /* TODO: Evaluate && left to right and stop if false? */
  if(dataLength > 2.0){
    if(data[(int)(dataLength - 2.0)] == '='){
      padding = padding + 1.0;
    }
  }
  quads = dataLength/4.0;

  /* TODO: Require init? */
  /* Init */
  decoded->data = (double*)malloc(sizeof(double) * (quads*3.0 - padding));
  decoded->dataLength = quads*3.0 - padding;
  decoded->errorMessage = (char*)malloc(sizeof(char) * (0.0));
  decoded->errorMessageLength = 0.0;
  decoded->success = true;

  done = false;
  for(i = 0.0; i < quads &&  !done ; i = i + 1.0){
    data4 = (char*)malloc(sizeof(char) * (4.0));
    data4Length = 4.0;
    currentQuad = i*4.0;
    currentTriplet = i*3.0;
    if(padding > 0.0 && (i + 1.0) == quads){
      if(padding == 2.0){
        data4[0] = data[(int)(currentQuad + 0.0)];
        data4[1] = data[(int)(currentQuad + 1.0)];
        data4[2] = 'A';
        data4[3] = 'A';
      }else if(padding == 1.0){
        data4[0] = data[(int)(currentQuad + 0.0)];
        data4[1] = data[(int)(currentQuad + 1.0)];
        data4[2] = data[(int)(currentQuad + 2.0)];
        data4[3] = 'A';
      }

    }else{
      data4[0] = data[(int)(currentQuad + 0.0)];
      data4[1] = data[(int)(currentQuad + 1.0)];
      data4[2] = data[(int)(currentQuad + 2.0)];
      data4[3] = data[(int)(currentQuad + 3.0)];
    }
    decoded3 = decode3table(data4, data4Length, table, tableLength, validTable, validTableLength);
    if(decoded3->success){
      if(padding > 0.0 && i + 1.0 == quads){
        if(padding == 2.0){
          decoded->data[(int)(currentTriplet + 0.0)] = decoded3->data[0];
        }else if(padding == 1.0){
          decoded->data[(int)(currentTriplet + 0.0)] = decoded3->data[0];
          decoded->data[(int)(currentTriplet + 1.0)] = decoded3->data[1];
        }

      }else{
        decoded->data[(int)(currentTriplet + 0.0)] = decoded3->data[0];
        decoded->data[(int)(currentTriplet + 1.0)] = decoded3->data[1];
        decoded->data[(int)(currentTriplet + 2.0)] = decoded3->data[2];
      }
    }else{
      decoded->success = false;
      decoded->errorMessage = decoded3->errorMessage;
      decoded->errorMessageLength = decoded3->errorMessageLength;
      done = true;
    }
    free(decoded3);
  }

  free(table);
  /* Unalloc 1 */
  return decoded;
}
Decoded *decode3(char *data, size_t dataLength){
  double *table;
  size_t tableLength;
  _Bool *validTable;
  size_t validTableLength;
  Decoded *decoded;

  table = getRevTable(&tableLength);
  /* Alloc 1 */
  validTable = getValidTable(&validTableLength);
  /* Alloc 2 */
  decoded = decode3table(data, dataLength, table, tableLength, validTable, validTableLength);

  free(validTable);
  /* Unalloc 2 */
  free(table);
  /* Unalloc 1 */
  return decoded;
}
Decoded *decode3table(char *data, size_t dataLength, double *table, size_t tableLength, _Bool *validTable, size_t validTableLength){
  double i, valid;
  Decoded *decoded;

  decoded = (Decoded *)malloc(sizeof(Decoded));
  /* Alloc r */
  decoded->success = false;
  decoded->data = (double*)malloc(sizeof(double) * (3.0));
  decoded->dataLength = 3.0;
  decoded->errorMessage = "";
  decoded->errorMessageLength = strlen(decoded->errorMessage);

  if(dataLength == 4.0){
    valid = 0.0;
    for(i = 0.0; i < 4.0; i = i + 1.0){
      if(isValidBase64characterTable(data[(int)(i)], validTable, validTableLength)){
        valid = valid + 1.0;
      }
    }
    if(valid == 4.0){
      decode3NoChecksTable(decoded, data, dataLength, table, tableLength);
      decoded->success = true;
    }else{
      decoded->errorMessage = "Invalid character in input string.";
      decoded->errorMessageLength = strlen(decoded->errorMessage);
      decoded->success = false;
    }
  }else{
    decoded->errorMessage = "There must be exactly four characters in the input string.";
    decoded->errorMessageLength = strlen(decoded->errorMessage);
    decoded->success = false;
  }

  return decoded;
}
_Bool isValidBase64characterTable(char c, _Bool *validTable, size_t validTableLength){

  return validTable[(int)(c)];
}
void decode3NoChecks(Decoded *decoded, char *data, size_t dataLength){
  double *table;
  size_t tableLength;

  table = getRevTable(&tableLength);
  decode3NoChecksTable(decoded, data, dataLength, table, tableLength);
}
void decode3NoChecksTable(Decoded *decoded, char *data, size_t dataLength, double *table, size_t tableLength){
  double total, i, n, r;

  total = 0.0;
  for(i = 0.0; i < 4.0; i = i + 1.0){
    n = getNumber(data[(int)(4.0 - i - 1.0)], table, tableLength);
    total = total + n*pow(2.0, i*6.0);
  }

  for(i = 0.0; i < 3.0; i = i + 1.0){
    r = fmod(total, pow(2.0, 8.0));
    decoded->data[(int)(3.0 - i - 1.0)] = r;
    total = (total - r)/pow(2.0, 8.0);
  }
}
double getNumber(char c, double *table, size_t tableLength){

  return table[(int)(c)];
}
Encoded *encode(double *data, size_t dataLength){
  Encoded *encoded, *encoded3;
  double padding, triplets, i, currentTriplet, currentQuad;
  double *data3;
  size_t data3Length;
  _Bool done;

  encoded = (Encoded *)malloc(sizeof(Encoded));

  padding = 0.0;
  if((fmod(dataLength, 3.0)) == 1.0){
    padding = 2.0;
  }
  if((fmod(dataLength, 3.0)) == 2.0){
    padding = 1.0;
  }
  triplets = ceil(dataLength/3.0);

  /* Init */
  encoded->data = (char*)malloc(sizeof(char) * (triplets*4.0));
  encoded->dataLength = triplets*4.0;
  encoded->errorMessage = (char*)malloc(sizeof(char) * (0.0));
  encoded->errorMessageLength = 0.0;
  encoded->success = true;

  done = false;
  for(i = 0.0; i < triplets &&  !done ; i = i + 1.0){
    data3 = (double*)malloc(sizeof(double) * (3.0));
    data3Length = 3.0;
    currentTriplet = i*3.0;
    currentQuad = i*4.0;
    if(padding > 0.0 && i + 1.0 == triplets){
      if(padding == 2.0){
        data3[0] = data[(int)(currentTriplet + 0.0)];
        data3[1] = 0.0;
        data3[2] = 0.0;
      }else if(padding == 1.0){
        data3[0] = data[(int)(currentTriplet + 0.0)];
        data3[1] = data[(int)(currentTriplet + 1.0)];
        data3[2] = 0.0;
      }

    }else{
      data3[0] = data[(int)(currentTriplet + 0.0)];
      data3[1] = data[(int)(currentTriplet + 1.0)];
      data3[2] = data[(int)(currentTriplet + 2.0)];
    }
    encoded3 = encode3(data3, data3Length);
    if(encoded3->success){
      encoded->data[(int)(currentQuad + 0.0)] = encoded3->data[0];
      encoded->data[(int)(currentQuad + 1.0)] = encoded3->data[1];
      if(padding > 0.0 && i + 1.0 == triplets){
        if(padding == 2.0){
          encoded->data[(int)(currentQuad + 2.0)] = '=';
          encoded->data[(int)(currentQuad + 3.0)] = '=';
        }else if(padding == 1.0){
          encoded->data[(int)(currentQuad + 2.0)] = encoded3->data[2];
          encoded->data[(int)(currentQuad + 3.0)] = '=';
        }

      }else{
        encoded->data[(int)(currentQuad + 2.0)] = encoded3->data[2];
        encoded->data[(int)(currentQuad + 3.0)] = encoded3->data[3];
      }
    }else{
      encoded->success = false;
      encoded->errorMessage = encoded3->errorMessage;
      encoded->errorMessageLength = encoded3->errorMessageLength;
      done = true;
    }
    free(encoded3);
  }

  return encoded;
}
Encoded *encode3(double *data, size_t dataLength){
  Encoded *encoded;
  double elementsVerified, i, e;
  _Bool isWithinBounds, isWhole;

  encoded = (Encoded *)malloc(sizeof(Encoded));
  /* Init */
  encoded->data = (char*)malloc(sizeof(char) * (4.0));
  encoded->dataLength = 4.0;
  encoded->data[0] = 'A';
  encoded->data[1] = 'A';
  encoded->data[2] = 'A';
  encoded->data[3] = 'A';
  encoded->errorMessage = (char*)malloc(sizeof(char) * (0.0));
  encoded->errorMessageLength = 0.0;
  encoded->success = false;

  /* Check. */
  if(dataLength == 3.0){
    elementsVerified = 0.0;
    for(i = 0.0; i < dataLength; i = i + 1.0){
      e = data[(int)(i)];
      isWithinBounds = (e >= 0.0) && (e < pow(2.0, 8.0));
      isWhole = (fmod(e, 1.0)) == 0.0;
      if(isWithinBounds && isWhole){
        elementsVerified = elementsVerified + 1.0;
      }else{
        encoded->errorMessage = "Input number is too high, too low or is not a whole number.";
        encoded->errorMessageLength = strlen(encoded->errorMessage);
        encoded->success = false;
      }
    }
    if(elementsVerified == dataLength){
      encode3NoChecks(encoded, data, dataLength);
      encoded->success = true;
    }
  }else{
    encoded->errorMessage = "Input must contain 3 numbers.";
    encoded->errorMessageLength = strlen(encoded->errorMessage);
    encoded->success = false;
  }

  return encoded;
}
void encode3NoChecks(Encoded *encoded, double *data, size_t dataLength){
  double total, i, bit6;
  char c;

  total = 0.0;
  for(i = 0.0; i < dataLength; i = i + 1.0){
    total = total + data[(int)(dataLength - i - 1.0)]*pow(2.0, i*8.0);
  }

  for(i = 0.0; i < 4.0; i = i + 1.0){
    bit6 = fmod(total, pow(2.0, 6.0));
    total = (total - bit6)/pow(2.0, 6.0);
    c = getCharacter(bit6);
    encoded->data[(int)(4.0 - i - 1.0)] = c;
  }
}
char getCharacter(double bit6){
  char *table;
  size_t tableLength;

  table = getTable(&tableLength);

  return table[(int)(bit6)];
}
char *getTable(size_t *returnArrayLength){

  *returnArrayLength = strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
  return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}
double *getRevTable(size_t *returnArrayLength){
  char *table;
  size_t tableLength;
  double i, max;
  double *revTable;
  size_t revTableLength;

  table = getTable(&tableLength);
  max = getMax(table, tableLength) + 1.0;
  revTable = (double*)malloc(sizeof(double) * (max + 1.0));
  revTableLength = max + 1.0;

  for(i = 0.0; i < tableLength; i = i + 1.0){
    revTable[(int)(table[(int)(i)])] = i;
  }

  *returnArrayLength = revTableLength;
  return revTable;
}
_Bool *getValidTable(size_t *returnArrayLength){
  char *table;
  size_t tableLength;
  double max, i;
  _Bool *validTable;
  size_t validTableLength;

  table = getTable(&tableLength);
  max = getMax(table, tableLength) + 1.0;
  validTable = (_Bool*)malloc(sizeof(_Bool) * (max));
  validTableLength = max;

  for(i = 0.0; i < max; i = i + 1.0){
    validTable[(int)(i)] = isValidBase64character(i);
  }

  *returnArrayLength = validTableLength;
  return validTable;
}
double getMax(char *table, size_t tableLength){
  double maxValue;
  double v;
  double i;

  maxValue = 0.0;

  for(i = 0.0; i < tableLength; i = i + 1.0){
    v = table[(int)(i)];
    maxValue = fmax(maxValue, v);
  }
  return maxValue;
}
_Bool isValidBase64character(char c){
  char *table;
  size_t tableLength;
  _Bool isValid;
  double i;

  table = getTable(&tableLength);
  isValid = false;

  for(i = 0.0; i < tableLength; i = i + 1.0){
    if(table[(int)(i)] == c){
      isValid = true;
    }
  }

  return isValid;
}
testresult *testDecoder(){
  char *orginal, *result;
  size_t orginalLength, resultLength;
  double *data;
  size_t dataLength;
  Encoded *encoded;
  Decoded *decoded;
  testresult *atestresult;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  orginal = "Hei";
  orginalLength = strlen(orginal);
  data = stringToNumberArray(&dataLength, orginal, orginalLength);
  encoded = encode(data, dataLength);

  decoded = decode3(encoded->data, encoded->dataLength);

  result = numberArrayToString(&resultLength, decoded->data, decoded->dataLength);

  if( !stringsEqual(orginal, orginalLength, result, resultLength) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(decoded->errorMessage, decoded->errorMessageLength);
  }

  return atestresult;
}
testresult *testDecoder2(){
  testresult *atestresult;
  double *original;
  size_t originalLength;
  Encoded *encoded;
  Decoded *decoded;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  original = (double*)malloc(sizeof(double) * (3.0));
  originalLength = 3.0;
  original[0] = 0.0;
  original[1] = pow(2.0, 4.0);
  original[2] = pow(2.0, 8.0) - 1.0;
  encoded = encode(original, originalLength);

  decoded = decode3(encoded->data, encoded->dataLength);

  if( !numberArraysEqual(original, originalLength, decoded->data, decoded->dataLength) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(decoded->errorMessage, decoded->errorMessageLength);
  }

  return atestresult;
}
testresult *testdecoderWithPadding(){
  testresult *atestresult;
  char *original;
  size_t originalLength;
  double *data;
  size_t dataLength;
  Encoded *encoded;
  Decoded *decoded;
  char *result;
  size_t resultLength;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  original = "Hei12";
  originalLength = strlen(original);
  data = stringToNumberArray(&dataLength, original, originalLength);
  encoded = encode(data, dataLength);

  decoded = decode(encoded->data, encoded->dataLength);

  result = numberArrayToString(&resultLength, decoded->data, decoded->dataLength);

  if( !stringsEqual(original, originalLength, result, resultLength) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(decoded->errorMessage, decoded->errorMessageLength);
  }

  return atestresult;
}
testresult *testDecoderWithPadding2(){
  testresult *atestresult;
  char *original;
  size_t originalLength;
  double *data;
  size_t dataLength;
  Encoded *encoded;
  Decoded *decoded;
  char *result;
  size_t resultLength;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  original = "Hei1";
  originalLength = strlen(original);
  data = stringToNumberArray(&dataLength, original, originalLength);
  encoded = encode(data, dataLength);

  decoded = decode(encoded->data, encoded->dataLength);

  result = numberArrayToString(&resultLength, decoded->data, decoded->dataLength);

  if( !stringsEqual(original, originalLength, result, resultLength) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(decoded->errorMessage, decoded->errorMessageLength);
  }

  return atestresult;
}
testresult *testDecoderReal1(){
  testresult *atestresult;
  Decoded *decoded;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  decoded = decode(strparam("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g="));

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(decoded->errorMessage, decoded->errorMessageLength);
  }

  return atestresult;
}
testresult *testEncoder(){
  testresult *atestresult;
  char *hei;
  size_t heiLength;
  double *data;
  size_t dataLength;
  Encoded *encoded;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 0.0);
  atestresult->errorMessagesLength = 0.0;

  hei = "Hei";
  heiLength = strlen(hei);

  data = stringToNumberArray(&dataLength, hei, heiLength);

  encoded = encode3(data, dataLength);

  if( !stringsEqual(encoded->data, encoded->dataLength, strparam("SGVp")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(encoded->errorMessage, encoded->errorMessageLength);
  }

  return atestresult;
}
string *stringFrom(char *src, size_t srcLength){
  string *str;

  str = (string *)malloc(sizeof(string));
  str->str = src;
  str->strLength = srcLength;
  return str;
}
testresult *testEncoder2(){
  testresult *atestresult;
  double *data;
  size_t dataLength;
  Encoded *encoded;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  /* TODO: Support direct instantiation. */
  data = (double*)malloc(sizeof(double) * (3.0));
  dataLength = 3.0;
  data[0] = 0.0;
  data[1] = pow(2.0, 4.0);
  data[2] = pow(2.0, 8.0) - 1.0;

  encoded = encode3(data, dataLength);

  if( !stringsEqual(encoded->data, encoded->dataLength, strparam("ABD/")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(encoded->errorMessage, encoded->errorMessageLength);
  }

  return atestresult;
}
testresult *testEncoderWithPadding(){
  testresult *atestresult;
  char *hei;
  size_t heiLength;
  double *data;
  size_t dataLength;
  Encoded *encoded;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  hei = "Hei12";
  heiLength = strlen(hei);

  data = stringToNumberArray(&dataLength, hei, heiLength);

  encoded = encode(data, dataLength);

  if( !stringsEqual(encoded->data, encoded->dataLength, strparam("SGVpMTI=")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(encoded->errorMessage, encoded->errorMessageLength);
  }

  return atestresult;
}
testresult *testEncoderWithPadding2(){
  testresult *atestresult;
  char *hei;
  size_t heiLength;
  double *data;
  size_t dataLength;
  Encoded *encoded;

  atestresult = (testresult *)malloc(sizeof(testresult));
  atestresult->failures = 0.0;
  atestresult->errorMessages = (string**)malloc(sizeof(string) * 1.0);
  atestresult->errorMessagesLength = 1.0;

  hei = "Hei1";
  heiLength = strlen(hei);

  data = stringToNumberArray(&dataLength, hei, heiLength);

  encoded = encode(data, dataLength);

  if( !stringsEqual(encoded->data, encoded->dataLength, strparam("SGVpMQ==")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    atestresult->errorMessages[0] = stringFrom(encoded->errorMessage, encoded->errorMessageLength);
  }

  return atestresult;
}
double test(){
  double failures;
  testresult *atestresult;

  failures = 0.0;

  atestresult = testDecoder();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testDecoder2();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testdecoderWithPadding();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testDecoderWithPadding2();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testEncoder();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testEncoder2();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testEncoderWithPadding();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testEncoderWithPadding2();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  atestresult = testDecoderReal1();
  if(atestresult->failures != 0.0){
    failures = failures + 1.0;
  }

  return failures;
}
double *stringToNumberArray(size_t *returnArrayLength, char *string, size_t stringLength){
  double i;
  double *array;
  size_t arrayLength;

  array = (double*)malloc(sizeof(double) * (stringLength));
  arrayLength = stringLength;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    array[(int)(i)] = string[(int)(i)];
  }
  *returnArrayLength = arrayLength;
  return array;
}
char *numberArrayToString(size_t *returnArrayLength, double *array, size_t arrayLength){
  double i;
  char *string;
  size_t stringLength;

  string = (char*)malloc(sizeof(char) * (arrayLength));
  stringLength = arrayLength;

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    string[(int)(i)] = array[(int)(i)];
  }
  *returnArrayLength = stringLength;
  return string;
}
_Bool stringsEqual(char *data1, size_t data1Length, char *data2, size_t data2Length){
  _Bool equal;
  double nrEqual, i;

  equal = false;
  if(data1Length == data2Length){
    nrEqual = 0.0;
    for(i = 0.0; i < data1Length; i = i + 1.0){
      if(data1[(int)(i)] == data2[(int)(i)]){
        nrEqual = nrEqual + 1.0;
      }
    }
    if(nrEqual == data1Length){
      equal = true;
    }
  }else{
    equal = false;
  }

  return equal;
}
_Bool numberArraysEqual(double *data1, size_t data1Length, double *data2, size_t data2Length){
  _Bool equal;
  double nrEqual, i;

  equal = false;
  if(data1Length == data2Length){
    nrEqual = 0.0;
    for(i = 0.0; i < data1Length; i = i + 1.0){
      if(data1[(int)(i)] == data2[(int)(i)]){
        nrEqual = nrEqual + 1.0;
      }
    }
    if(nrEqual == data1Length){
      equal = true;
    }
  }else{
    equal = false;
  }

  return equal;
}


#include <stdio.h>

int main(){
	printf("%f\n", test());
	return 0;
}