#include <cmath>
#include <cstring>
#include <vector>

using namespace std;

#define toVector(s) (new vector<char> ((s), (s) + strlen(s)))

struct Decoded;

struct Encoded;

struct string;

struct testresult;

struct Decoded{
  vector<double> *data;
  bool success;
  vector<char> *errorMessage;
};

struct Encoded{
  vector<char> *data;
  bool success;
  vector<char> *errorMessage;
};

struct string{
  vector<char> *str;
};

struct testresult{
  double failures;
  vector<string*> *errorMessages;
};


Decoded *decode(vector<char> *data);
Decoded *decode3(vector<char> *data);
Decoded *decode3table(vector<char> *data, vector<double> *table, vector<bool> *validTable);
bool isValidBase64characterTable(char c, vector<bool> *validTable);
void decode3NoChecks(Decoded *decoded, vector<char> *data);
void decode3NoChecksTable(Decoded *decoded, vector<char> *data, vector<double> *table);
double getNumber(char c, vector<double> *table);

Encoded *encode(vector<double> *data);
Encoded *encode3(vector<double> *data);
void encode3NoChecks(Encoded *encoded, vector<double> *data);
char getCharacter(double bit6);

vector<char> *getTable();
vector<double> *getRevTable();
vector<bool> *getValidTable();
double getMax(vector<char> *table);
bool isValidBase64character(char c);

testresult *testDecoder();
testresult *testDecoder2();
testresult *testdecoderWithPadding();
testresult *testDecoderWithPadding2();
testresult *testDecoderReal1();

testresult *testEncoder();
string *stringFrom(vector<char> *src);
testresult *testEncoder2();
testresult *testEncoderWithPadding();
testresult *testEncoderWithPadding2();

double test();

vector<double> *stringToNumberArray(vector<char> *string);
vector<char> *numberArrayToString(vector<double> *array);
bool stringsEqual(vector<char> *data1, vector<char> *data2);
bool numberArraysEqual(vector<double> *data1, vector<double> *data2);

Decoded *decode(vector<char> *data){
  bool done;
  vector<char> *data4;
  double quads, i, currentQuad, currentTriplet, padding;
  Decoded *decoded3, *decoded;
  vector<double> *table;
  vector<bool> *validTable;

  decoded = new Decoded();
  table = getRevTable();
  /* Alloc 1 */
  validTable = getValidTable();
  /* Alloc 3 */
  padding = 0.0;
  if(data->size() > 1.0){
    if((*data)[(int)(data->size() - 1.0)] == '='){
      padding = padding + 1.0;
    }
  }
  /* TODO: Evaluate && left to right and stop if false? */
  if(data->size() > 2.0){
    if((*data)[(int)(data->size() - 2.0)] == '='){
      padding = padding + 1.0;
    }
  }
  quads = data->size()/4.0;

  /* TODO: Require init? */
  /* Init */
  decoded->data = new vector<double> (quads*3.0 - padding);
  decoded->errorMessage = new vector<char> (0.0);
  decoded->success = true;

  done = false;
  for(i = 0.0; i < quads &&  !done ; i = i + 1.0){
    data4 = new vector<char> (4.0);
    currentQuad = i*4.0;
    currentTriplet = i*3.0;
    if(padding > 0.0 && (i + 1.0) == quads){
      if(padding == 2.0){
        (*data4)[0] = (*data)[(int)(currentQuad + 0.0)];
        (*data4)[1] = (*data)[(int)(currentQuad + 1.0)];
        (*data4)[2] = 'A';
        (*data4)[3] = 'A';
      }else if(padding == 1.0){
        (*data4)[0] = (*data)[(int)(currentQuad + 0.0)];
        (*data4)[1] = (*data)[(int)(currentQuad + 1.0)];
        (*data4)[2] = (*data)[(int)(currentQuad + 2.0)];
        (*data4)[3] = 'A';
      }

    }else{
      (*data4)[0] = (*data)[(int)(currentQuad + 0.0)];
      (*data4)[1] = (*data)[(int)(currentQuad + 1.0)];
      (*data4)[2] = (*data)[(int)(currentQuad + 2.0)];
      (*data4)[3] = (*data)[(int)(currentQuad + 3.0)];
    }
    decoded3 = decode3table(data4, table, validTable);
    if(decoded3->success){
      if(padding > 0.0 && i + 1.0 == quads){
        if(padding == 2.0){
          (*decoded->data)[(int)(currentTriplet + 0.0)] = (*decoded3->data)[0];
        }else if(padding == 1.0){
          (*decoded->data)[(int)(currentTriplet + 0.0)] = (*decoded3->data)[0];
          (*decoded->data)[(int)(currentTriplet + 1.0)] = (*decoded3->data)[1];
        }

      }else{
        (*decoded->data)[(int)(currentTriplet + 0.0)] = (*decoded3->data)[0];
        (*decoded->data)[(int)(currentTriplet + 1.0)] = (*decoded3->data)[1];
        (*decoded->data)[(int)(currentTriplet + 2.0)] = (*decoded3->data)[2];
      }
    }else{
      decoded->success = false;
      decoded->errorMessage = decoded3->errorMessage;
      done = true;
    }
    delete decoded3;
  }

  delete table;
  /* Unalloc 1 */
  return decoded;
}
Decoded *decode3(vector<char> *data){
  vector<double> *table;
  vector<bool> *validTable;
  Decoded *decoded;

  table = getRevTable();
  /* Alloc 1 */
  validTable = getValidTable();
  /* Alloc 2 */
  decoded = decode3table(data, table, validTable);

  delete validTable;
  /* Unalloc 2 */
  delete table;
  /* Unalloc 1 */
  return decoded;
}
Decoded *decode3table(vector<char> *data, vector<double> *table, vector<bool> *validTable){
  double i, valid;
  Decoded *decoded;

  decoded = new Decoded();
  /* Alloc r */
  decoded->success = false;
  decoded->data = new vector<double> (3.0);
  decoded->errorMessage = toVector("");

  if(data->size() == 4.0){
    valid = 0.0;
    for(i = 0.0; i < 4.0; i = i + 1.0){
      if(isValidBase64characterTable((*data)[(int)(i)], validTable)){
        valid = valid + 1.0;
      }
    }
    if(valid == 4.0){
      decode3NoChecksTable(decoded, data, table);
      decoded->success = true;
    }else{
      decoded->errorMessage = toVector("Invalid character in input string.");
      decoded->success = false;
    }
  }else{
    decoded->errorMessage = toVector("There must be exactly four characters in the input string.");
    decoded->success = false;
  }

  return decoded;
}
bool isValidBase64characterTable(char c, vector<bool> *validTable){

  return (*validTable)[(int)(c)];
}
void decode3NoChecks(Decoded *decoded, vector<char> *data){
  vector<double> *table;

  table = getRevTable();
  decode3NoChecksTable(decoded, data, table);
}
void decode3NoChecksTable(Decoded *decoded, vector<char> *data, vector<double> *table){
  double total, i, n, r;

  total = 0.0;
  for(i = 0.0; i < 4.0; i = i + 1.0){
    n = getNumber((*data)[(int)(4.0 - i - 1.0)], table);
    total = total + n*pow(2.0, i*6.0);
  }

  for(i = 0.0; i < 3.0; i = i + 1.0){
    r = fmod(total, pow(2.0, 8.0));
    (*decoded->data)[(int)(3.0 - i - 1.0)] = r;
    total = (total - r)/pow(2.0, 8.0);
  }
}
double getNumber(char c, vector<double> *table){

  return (*table)[(int)(c)];
}
Encoded *encode(vector<double> *data){
  Encoded *encoded, *encoded3;
  double padding, triplets, i, currentTriplet, currentQuad;
  vector<double> *data3;
  bool done;

  encoded = new Encoded();

  padding = 0.0;
  if((fmod(data->size(), 3.0)) == 1.0){
    padding = 2.0;
  }
  if((fmod(data->size(), 3.0)) == 2.0){
    padding = 1.0;
  }
  triplets = ceil(data->size()/3.0);

  /* Init */
  encoded->data = new vector<char> (triplets*4.0);
  encoded->errorMessage = new vector<char> (0.0);
  encoded->success = true;

  done = false;
  for(i = 0.0; i < triplets &&  !done ; i = i + 1.0){
    data3 = new vector<double> (3.0);
    currentTriplet = i*3.0;
    currentQuad = i*4.0;
    if(padding > 0.0 && i + 1.0 == triplets){
      if(padding == 2.0){
        (*data3)[0] = (*data)[(int)(currentTriplet + 0.0)];
        (*data3)[1] = 0.0;
        (*data3)[2] = 0.0;
      }else if(padding == 1.0){
        (*data3)[0] = (*data)[(int)(currentTriplet + 0.0)];
        (*data3)[1] = (*data)[(int)(currentTriplet + 1.0)];
        (*data3)[2] = 0.0;
      }

    }else{
      (*data3)[0] = (*data)[(int)(currentTriplet + 0.0)];
      (*data3)[1] = (*data)[(int)(currentTriplet + 1.0)];
      (*data3)[2] = (*data)[(int)(currentTriplet + 2.0)];
    }
    encoded3 = encode3(data3);
    if(encoded3->success){
      (*encoded->data)[(int)(currentQuad + 0.0)] = (*encoded3->data)[0];
      (*encoded->data)[(int)(currentQuad + 1.0)] = (*encoded3->data)[1];
      if(padding > 0.0 && i + 1.0 == triplets){
        if(padding == 2.0){
          (*encoded->data)[(int)(currentQuad + 2.0)] = '=';
          (*encoded->data)[(int)(currentQuad + 3.0)] = '=';
        }else if(padding == 1.0){
          (*encoded->data)[(int)(currentQuad + 2.0)] = (*encoded3->data)[2];
          (*encoded->data)[(int)(currentQuad + 3.0)] = '=';
        }

      }else{
        (*encoded->data)[(int)(currentQuad + 2.0)] = (*encoded3->data)[2];
        (*encoded->data)[(int)(currentQuad + 3.0)] = (*encoded3->data)[3];
      }
    }else{
      encoded->success = false;
      encoded->errorMessage = encoded3->errorMessage;
      done = true;
    }
    delete encoded3;
  }

  return encoded;
}
Encoded *encode3(vector<double> *data){
  Encoded *encoded;
  double elementsVerified, i, e;
  bool isWithinBounds, isWhole;

  encoded = new Encoded();
  /* Init */
  encoded->data = new vector<char> (4.0);
  (*encoded->data)[0] = 'A';
  (*encoded->data)[1] = 'A';
  (*encoded->data)[2] = 'A';
  (*encoded->data)[3] = 'A';
  encoded->errorMessage = new vector<char> (0.0);
  encoded->success = false;

  /* Check. */
  if(data->size() == 3.0){
    elementsVerified = 0.0;
    for(i = 0.0; i < data->size(); i = i + 1.0){
      e = (*data)[(int)(i)];
      isWithinBounds = (e >= 0.0) && (e < pow(2.0, 8.0));
      isWhole = (fmod(e, 1.0)) == 0.0;
      if(isWithinBounds && isWhole){
        elementsVerified = elementsVerified + 1.0;
      }else{
        encoded->errorMessage = toVector("Input number is too high, too low or is not a whole number.");
        encoded->success = false;
      }
    }
    if(elementsVerified == data->size()){
      encode3NoChecks(encoded, data);
      encoded->success = true;
    }
  }else{
    encoded->errorMessage = toVector("Input must contain 3 numbers.");
    encoded->success = false;
  }

  return encoded;
}
void encode3NoChecks(Encoded *encoded, vector<double> *data){
  double total, i, bit6;
  char c;

  total = 0.0;
  for(i = 0.0; i < data->size(); i = i + 1.0){
    total = total + (*data)[(int)(data->size() - i - 1.0)]*pow(2.0, i*8.0);
  }

  for(i = 0.0; i < 4.0; i = i + 1.0){
    bit6 = fmod(total, pow(2.0, 6.0));
    total = (total - bit6)/pow(2.0, 6.0);
    c = getCharacter(bit6);
    (*encoded->data)[(int)(4.0 - i - 1.0)] = c;
  }
}
char getCharacter(double bit6){
  vector<char> *table;

  table = getTable();

  return (*table)[(int)(bit6)];
}
vector<char> *getTable(){

  return toVector("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
}
vector<double> *getRevTable(){
  vector<char> *table;
  double i, max;
  vector<double> *revTable;

  table = getTable();
  max = getMax(table) + 1.0;
  revTable = new vector<double> (max + 1.0);

  for(i = 0.0; i < table->size(); i = i + 1.0){
    (*revTable)[(int)((*table)[(int)(i)])] = i;
  }

  return revTable;
}
vector<bool> *getValidTable(){
  vector<char> *table;
  double max, i;
  vector<bool> *validTable;

  table = getTable();
  max = getMax(table) + 1.0;
  validTable = new vector<bool> (max);

  for(i = 0.0; i < max; i = i + 1.0){
    (*validTable)[(int)(i)] = isValidBase64character(i);
  }

  return validTable;
}
double getMax(vector<char> *table){
  double maxValue;
  double v;
  double i;

  maxValue = 0.0;

  for(i = 0.0; i < table->size(); i = i + 1.0){
    v = (*table)[(int)(i)];
    maxValue = fmax(maxValue, v);
  }
  return maxValue;
}
bool isValidBase64character(char c){
  vector<char> *table;
  bool isValid;
  double i;

  table = getTable();
  isValid = false;

  for(i = 0.0; i < table->size(); i = i + 1.0){
    if((*table)[(int)(i)] == c){
      isValid = true;
    }
  }

  return isValid;
}
testresult *testDecoder(){
  vector<char> *orginal, *result;
  vector<double> *data;
  Encoded *encoded;
  Decoded *decoded;
  testresult *atestresult;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  orginal = toVector("Hei");
  data = stringToNumberArray(orginal);
  encoded = encode(data);

  decoded = decode3(encoded->data);

  result = numberArrayToString(decoded->data);

  if( !stringsEqual(orginal, result) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(decoded->errorMessage);
  }

  return atestresult;
}
testresult *testDecoder2(){
  testresult *atestresult;
  vector<double> *original;
  Encoded *encoded;
  Decoded *decoded;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  original = new vector<double> (3.0);
  (*original)[0] = 0.0;
  (*original)[1] = pow(2.0, 4.0);
  (*original)[2] = pow(2.0, 8.0) - 1.0;
  encoded = encode(original);

  decoded = decode3(encoded->data);

  if( !numberArraysEqual(original, decoded->data) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(decoded->errorMessage);
  }

  return atestresult;
}
testresult *testdecoderWithPadding(){
  testresult *atestresult;
  vector<char> *original;
  vector<double> *data;
  Encoded *encoded;
  Decoded *decoded;
  vector<char> *result;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  original = toVector("Hei12");
  data = stringToNumberArray(original);
  encoded = encode(data);

  decoded = decode(encoded->data);

  result = numberArrayToString(decoded->data);

  if( !stringsEqual(original, result) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(decoded->errorMessage);
  }

  return atestresult;
}
testresult *testDecoderWithPadding2(){
  testresult *atestresult;
  vector<char> *original;
  vector<double> *data;
  Encoded *encoded;
  Decoded *decoded;
  vector<char> *result;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  original = toVector("Hei1");
  data = stringToNumberArray(original);
  encoded = encode(data);

  decoded = decode(encoded->data);

  result = numberArrayToString(decoded->data);

  if( !stringsEqual(original, result) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(decoded->errorMessage);
  }

  return atestresult;
}
testresult *testDecoderReal1(){
  testresult *atestresult;
  Decoded *decoded;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  decoded = decode(toVector("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g="));

  if( !decoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(decoded->errorMessage);
  }

  return atestresult;
}
testresult *testEncoder(){
  testresult *atestresult;
  vector<char> *hei;
  vector<double> *data;
  Encoded *encoded;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (0.0);

  hei = toVector("Hei");

  data = stringToNumberArray(hei);

  encoded = encode3(data);

  if( !stringsEqual(encoded->data, toVector("SGVp")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(encoded->errorMessage);
  }

  return atestresult;
}
string *stringFrom(vector<char> *src){
  string *str;

  str = new string();
  str->str = src;
  return str;
}
testresult *testEncoder2(){
  testresult *atestresult;
  vector<double> *data;
  Encoded *encoded;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  /* TODO: Support direct instantiation. */
  data = new vector<double> (3.0);
  (*data)[0] = 0.0;
  (*data)[1] = pow(2.0, 4.0);
  (*data)[2] = pow(2.0, 8.0) - 1.0;

  encoded = encode3(data);

  if( !stringsEqual(encoded->data, toVector("ABD/")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(encoded->errorMessage);
  }

  return atestresult;
}
testresult *testEncoderWithPadding(){
  testresult *atestresult;
  vector<char> *hei;
  vector<double> *data;
  Encoded *encoded;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  hei = toVector("Hei12");

  data = stringToNumberArray(hei);

  encoded = encode(data);

  if( !stringsEqual(encoded->data, toVector("SGVpMTI=")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(encoded->errorMessage);
  }

  return atestresult;
}
testresult *testEncoderWithPadding2(){
  testresult *atestresult;
  vector<char> *hei;
  vector<double> *data;
  Encoded *encoded;

  atestresult = new testresult();
  atestresult->failures = 0.0;
  atestresult->errorMessages = new vector<string*> (1.0);

  hei = toVector("Hei1");

  data = stringToNumberArray(hei);

  encoded = encode(data);

  if( !stringsEqual(encoded->data, toVector("SGVpMQ==")) ){
    atestresult->failures = atestresult->failures + 1.0;
  }

  if( !encoded->success ){
    atestresult->failures = atestresult->failures + 1.0;
    (*atestresult->errorMessages)[0] = stringFrom(encoded->errorMessage);
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
vector<double> *stringToNumberArray(vector<char> *string){
  double i;
  vector<double> *array;

  array = new vector<double> (string->size());

  for(i = 0.0; i < string->size(); i = i + 1.0){
    (*array)[(int)(i)] = (*string)[(int)(i)];
  }
  return array;
}
vector<char> *numberArrayToString(vector<double> *array){
  double i;
  vector<char> *string;

  string = new vector<char> (array->size());

  for(i = 0.0; i < array->size(); i = i + 1.0){
    (*string)[(int)(i)] = (*array)[(int)(i)];
  }
  return string;
}
bool stringsEqual(vector<char> *data1, vector<char> *data2){
  bool equal;
  double nrEqual, i;

  equal = false;
  if(data1->size() == data2->size()){
    nrEqual = 0.0;
    for(i = 0.0; i < data1->size(); i = i + 1.0){
      if((*data1)[(int)(i)] == (*data2)[(int)(i)]){
        nrEqual = nrEqual + 1.0;
      }
    }
    if(nrEqual == data1->size()){
      equal = true;
    }
  }else{
    equal = false;
  }

  return equal;
}
bool numberArraysEqual(vector<double> *data1, vector<double> *data2){
  bool equal;
  double nrEqual, i;

  equal = false;
  if(data1->size() == data2->size()){
    nrEqual = 0.0;
    for(i = 0.0; i < data1->size(); i = i + 1.0){
      if((*data1)[(int)(i)] == (*data2)[(int)(i)]){
        nrEqual = nrEqual + 1.0;
      }
    }
    if(nrEqual == data1->size()){
      equal = true;
    }
  }else{
    equal = false;
  }

  return equal;
}


#include <iostream>

using namespace std;

int main(){
	cout << test();
	return 0;
}