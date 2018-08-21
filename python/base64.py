from math import *
class Decoded:
  data = None
  success = None
  errorMessage = None

class Encoded:
  data = None
  success = None
  errorMessage = None

class string:
  str = None

class testresult:
  failures = None
  errorMessages = None

def decode(data):
  decoded = Decoded()
  table = getRevTable()
  # Alloc 1
  validTable = getValidTable()
  # Alloc 3
  padding = 0.0
  if len(data) > 1.0:
    if data[int(len(data) - 1.0)] == '=':
      padding = padding + 1.0
  # TODO: Evaluate && left to right and stop if false?
  if len(data) > 2.0:
    if data[int(len(data) - 2.0)] == '=':
      padding = padding + 1.0
  quads = len(data)/4.0

  # TODO: Require init?
  # Init
  decoded.data =  [None]*int(quads*3.0 - padding)
  decoded.errorMessage =  [None]*int(0.0)
  decoded.success = True

  done = False
  i = 0.0
  while i < quads and  not done :
    data4 =  [None]*int(4.0)
    currentQuad = i*4.0
    currentTriplet = i*3.0
    if padding > 0.0 and (i + 1.0) == quads:
      if padding == 2.0:
        data4[int(0.0)] = data[int(currentQuad + 0.0)]
        data4[int(1.0)] = data[int(currentQuad + 1.0)]
        data4[int(2.0)] = 'A'
        data4[int(3.0)] = 'A'
      elif padding == 1.0:
        data4[int(0.0)] = data[int(currentQuad + 0.0)]
        data4[int(1.0)] = data[int(currentQuad + 1.0)]
        data4[int(2.0)] = data[int(currentQuad + 2.0)]
        data4[int(3.0)] = 'A'
    else:
      data4[int(0.0)] = data[int(currentQuad + 0.0)]
      data4[int(1.0)] = data[int(currentQuad + 1.0)]
      data4[int(2.0)] = data[int(currentQuad + 2.0)]
      data4[int(3.0)] = data[int(currentQuad + 3.0)]
    decoded3 = decode3table(data4, table, validTable)
    if decoded3.success:
      if padding > 0.0 and i + 1.0 == quads:
        if padding == 2.0:
          decoded.data[int(currentTriplet + 0.0)] = decoded3.data[int(0.0)]
        elif padding == 1.0:
          decoded.data[int(currentTriplet + 0.0)] = decoded3.data[int(0.0)]
          decoded.data[int(currentTriplet + 1.0)] = decoded3.data[int(1.0)]
      else:
        decoded.data[int(currentTriplet + 0.0)] = decoded3.data[int(0.0)]
        decoded.data[int(currentTriplet + 1.0)] = decoded3.data[int(1.0)]
        decoded.data[int(currentTriplet + 2.0)] = decoded3.data[int(2.0)]
    else:
      decoded.success = False
      decoded.errorMessage = decoded3.errorMessage
      done = True
    del(decoded3)
    i = i + 1.0
  

  del(table)
  # Unalloc 1
  return decoded

def decode3(data):
  table = getRevTable()
  # Alloc 1
  validTable = getValidTable()
  # Alloc 2
  decoded = decode3table(data, table, validTable)

  del(validTable)
  # Unalloc 2
  del(table)
  # Unalloc 1
  return decoded

def decode3table(data, table, validTable):
  decoded = Decoded()
  # Alloc r
  decoded.success = False
  decoded.data =  [None]*int(3.0)
  decoded.errorMessage = ""

  if len(data) == 4.0:
    valid = 0.0
    i = 0.0
    while i < 4.0:
      if isValidBase64characterTable(data[int(i)], validTable):
        valid = valid + 1.0
      i = i + 1.0
    
    if valid == 4.0:
      decode3NoChecksTable(decoded, data, table)
      decoded.success = True
    else:
      decoded.errorMessage = "Invalid character in input string."
      decoded.success = False
  else:
    decoded.errorMessage = "There must be exactly four characters in the input string."
    decoded.success = False

  return decoded

def isValidBase64characterTable(c, validTable):
  return validTable[int(ord(c))]

def decode3NoChecks(decoded, data):
  table = getRevTable()
  decode3NoChecksTable(decoded, data, table)

def decode3NoChecksTable(decoded, data, table):
  total = 0.0
  i = 0.0
  while i < 4.0:
    n = getNumber(data[int(4.0 - i - 1.0)], table)
    total = total + n*2.0**(i*6.0)
    i = i + 1.0
  

  i = 0.0
  while i < 3.0:
    r = total % 2.0**8.0
    decoded.data[int(3.0 - i - 1.0)] = r
    total = (total - r)/2.0**8.0
    i = i + 1.0
  

def getNumber(c, table):
  return table[int(ord(c))]

def encode(data):
  encoded = Encoded()

  padding = 0.0
  if (len(data) % 3.0) == 1.0:
    padding = 2.0
  if (len(data) % 3.0) == 2.0:
    padding = 1.0
  triplets = ceil(len(data)/3.0)

  # Init
  encoded.data =  [None]*int(triplets*4.0)
  encoded.errorMessage =  [None]*int(0.0)
  encoded.success = True

  done = False
  i = 0.0
  while i < triplets and  not done :
    data3 =  [None]*int(3.0)
    currentTriplet = i*3.0
    currentQuad = i*4.0
    if padding > 0.0 and i + 1.0 == triplets:
      if padding == 2.0:
        data3[int(0.0)] = data[int(currentTriplet + 0.0)]
        data3[int(1.0)] = 0.0
        data3[int(2.0)] = 0.0
      elif padding == 1.0:
        data3[int(0.0)] = data[int(currentTriplet + 0.0)]
        data3[int(1.0)] = data[int(currentTriplet + 1.0)]
        data3[int(2.0)] = 0.0
    else:
      data3[int(0.0)] = data[int(currentTriplet + 0.0)]
      data3[int(1.0)] = data[int(currentTriplet + 1.0)]
      data3[int(2.0)] = data[int(currentTriplet + 2.0)]
    encoded3 = encode3(data3)
    if encoded3.success:
      encoded.data[int(currentQuad + 0.0)] = encoded3.data[int(0.0)]
      encoded.data[int(currentQuad + 1.0)] = encoded3.data[int(1.0)]
      if padding > 0.0 and i + 1.0 == triplets:
        if padding == 2.0:
          encoded.data[int(currentQuad + 2.0)] = '='
          encoded.data[int(currentQuad + 3.0)] = '='
        elif padding == 1.0:
          encoded.data[int(currentQuad + 2.0)] = encoded3.data[int(2.0)]
          encoded.data[int(currentQuad + 3.0)] = '='
      else:
        encoded.data[int(currentQuad + 2.0)] = encoded3.data[int(2.0)]
        encoded.data[int(currentQuad + 3.0)] = encoded3.data[int(3.0)]
    else:
      encoded.success = False
      encoded.errorMessage = encoded3.errorMessage
      done = True
    del(encoded3)
    i = i + 1.0
  

  return encoded

def encode3(data):
  encoded = Encoded()
  # Init
  encoded.data =  [None]*int(4.0)
  encoded.data[int(0.0)] = 'A'
  encoded.data[int(1.0)] = 'A'
  encoded.data[int(2.0)] = 'A'
  encoded.data[int(3.0)] = 'A'
  encoded.errorMessage =  [None]*int(0.0)
  encoded.success = False

  # Check.
  if len(data) == 3.0:
    elementsVerified = 0.0
    i = 0.0
    while i < len(data):
      e = data[int(i)]
      isWithinBounds = (e >= 0.0) and (e < 2.0**8.0)
      isWhole = (e % 1.0) == 0.0
      if isWithinBounds and isWhole:
        elementsVerified = elementsVerified + 1.0
      else:
        encoded.errorMessage = "Input number is too high, too low or is not a whole number."
        encoded.success = False
      i = i + 1.0
    
    if elementsVerified == len(data):
      encode3NoChecks(encoded, data)
      encoded.success = True
  else:
    encoded.errorMessage = "Input must contain 3 numbers."
    encoded.success = False

  return encoded

def encode3NoChecks(encoded, data):
  total = 0.0
  i = 0.0
  while i < len(data):
    total = total + data[int(len(data) - i - 1.0)]*2.0**(i*8.0)
    i = i + 1.0
  

  i = 0.0
  while i < 4.0:
    bit6 = total % 2.0**6.0
    total = (total - bit6)/2.0**6.0
    c = getCharacter(bit6)
    encoded.data[int(4.0 - i - 1.0)] = c
    i = i + 1.0
  

def getCharacter(bit6):
  table = getTable()

  return table[int(bit6)]

def getTable():
  return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

def getRevTable():
  table = getTable()
  max = getMax(table) + 1.0
  revTable =  [None]*int(max + 1.0)

  i = 0.0
  while i < len(table):
    revTable[int(ord(table[int(i)]))] = i
    i = i + 1.0
  

  return revTable

def getValidTable():
  table = getTable()
  max = getMax(table) + 1.0
  validTable =  [None]*int(max)

  i = 0.0
  while i < max:
    validTable[int(i)] = isValidBase64character(chr(int(i)))
    i = i + 1.0
  

  return validTable

def getMax(table):
  maxValue = 0.0

  i = 0.0
  while i < len(table):
    v = ord(table[int(i)])
    maxValue = max(maxValue,v)
    i = i + 1.0
  
  return maxValue

def isValidBase64character(c):
  table = getTable()
  isValid = False

  i = 0.0
  while i < len(table):
    if table[int(i)] == c:
      isValid = True
    i = i + 1.0
  

  return isValid

def testDecoder():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  orginal = "Hei"
  data = stringToNumberArray(orginal)
  encoded = encode(data)

  decoded = decode3(encoded.data)

  result = numberArrayToString(decoded.data)

  if  not stringsEqual(orginal, result) :
    atestresult.failures = atestresult.failures + 1.0

  if  not decoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(decoded.errorMessage)

  return atestresult

def testDecoder2():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  original =  [None]*int(3.0)
  original[int(0.0)] = 0.0
  original[int(1.0)] = 2.0**4.0
  original[int(2.0)] = 2.0**8.0 - 1.0
  encoded = encode(original)

  decoded = decode3(encoded.data)

  if  not numberArraysEqual(original, decoded.data) :
    atestresult.failures = atestresult.failures + 1.0

  if  not decoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(decoded.errorMessage)

  return atestresult

def testdecoderWithPadding():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  original = "Hei12"
  data = stringToNumberArray(original)
  encoded = encode(data)

  decoded = decode(encoded.data)

  result = numberArrayToString(decoded.data)

  if  not stringsEqual(original, result) :
    atestresult.failures = atestresult.failures + 1.0

  if  not decoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(decoded.errorMessage)

  return atestresult

def testDecoderWithPadding2():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  original = "Hei1"
  data = stringToNumberArray(original)
  encoded = encode(data)

  decoded = decode(encoded.data)

  result = numberArrayToString(decoded.data)

  if  not stringsEqual(original, result) :
    atestresult.failures = atestresult.failures + 1.0

  if  not decoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(decoded.errorMessage)

  return atestresult

def testDecoderReal1():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  decoded = decode("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g=")

  if  not decoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(decoded.errorMessage)

  return atestresult

def testEncoder():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(0.0)

  hei = "Hei"

  data = stringToNumberArray(hei)

  encoded = encode3(data)

  if  not stringsEqual(encoded.data, "SGVp") :
    atestresult.failures = atestresult.failures + 1.0

  if  not encoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(encoded.errorMessage)

  return atestresult

def stringFrom(src):
  str = string()
  str.str = src
  return str

def testEncoder2():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  # TODO: Support direct instantiation.
  data =  [None]*int(3.0)
  data[int(0.0)] = 0.0
  data[int(1.0)] = 2.0**4.0
  data[int(2.0)] = 2.0**8.0 - 1.0

  encoded = encode3(data)

  if  not stringsEqual(encoded.data, "ABD/") :
    atestresult.failures = atestresult.failures + 1.0

  if  not encoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(encoded.errorMessage)

  return atestresult

def testEncoderWithPadding():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  hei = "Hei12"

  data = stringToNumberArray(hei)

  encoded = encode(data)

  if  not stringsEqual(encoded.data, "SGVpMTI=") :
    atestresult.failures = atestresult.failures + 1.0

  if  not encoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(encoded.errorMessage)

  return atestresult

def testEncoderWithPadding2():
  atestresult = testresult()
  atestresult.failures = 0.0
  atestresult.errorMessages =  [None]*int(1.0)

  hei = "Hei1"

  data = stringToNumberArray(hei)

  encoded = encode(data)

  if  not stringsEqual(encoded.data, "SGVpMQ==") :
    atestresult.failures = atestresult.failures + 1.0

  if  not encoded.success :
    atestresult.failures = atestresult.failures + 1.0
    atestresult.errorMessages[int(0.0)] = stringFrom(encoded.errorMessage)

  return atestresult

def test():
  failures = 0.0

  atestresult = testDecoder()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testDecoder2()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testdecoderWithPadding()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testDecoderWithPadding2()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testEncoder()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testEncoder2()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testEncoderWithPadding()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testEncoderWithPadding2()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  atestresult = testDecoderReal1()
  if atestresult.failures != 0.0:
    failures = failures + 1.0

  return failures

def stringToNumberArray(string):
  array =  [None]*int(len(string))

  i = 0.0
  while i < len(string):
    array[int(i)] = ord(string[int(i)])
    i = i + 1.0
  
  return array

def numberArrayToString(array):
  string =  [None]*int(len(array))

  i = 0.0
  while i < len(array):
    string[int(i)] = chr(int(array[int(i)]))
    i = i + 1.0
  
  return string

def stringsEqual(data1, data2):
  equal = False
  if len(data1) == len(data2):
    nrEqual = 0.0
    i = 0.0
    while i < len(data1):
      if data1[int(i)] == data2[int(i)]:
        nrEqual = nrEqual + 1.0
      i = i + 1.0
    
    if nrEqual == len(data1):
      equal = True
  else:
    equal = False

  return equal

def numberArraysEqual(data1, data2):
  equal = False
  if len(data1) == len(data2):
    nrEqual = 0.0
    i = 0.0
    while i < len(data1):
      if data1[int(i)] == data2[int(i)]:
        nrEqual = nrEqual + 1.0
      i = i + 1.0
    
    if nrEqual == len(data1):
      equal = True
  else:
    equal = False

  return equal



print(test())