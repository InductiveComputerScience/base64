import static java.lang.Math.*;

public class base64{
static public class Decoded{
	public double [] data;
	public boolean success;
	public char [] errorMessage;
}
static public class Encoded{
	public char [] data;
	public boolean success;
	public char [] errorMessage;
}
static public class string{
	public char [] str;
}
static public class testresult{
	public double failures;
	public string [] errorMessages;
}
	public static Decoded decode(char [] data){
		boolean done;
		char [] data4;
		double quads, i, currentQuad, currentTriplet, padding;
		Decoded decoded3, decoded;
		double [] table;
		boolean [] validTable;

		decoded = new Decoded();
		table = getRevTable();
		/* Alloc 1*/
		validTable = getValidTable();
		/* Alloc 3*/
		padding = 0d;
		if(data.length > 1d){
			if(data[(int)(data.length - 1d)] == '='){
				padding = padding + 1d;
			}
		}
		/* TODO: Evaluate && left to right and stop if false?*/
		if(data.length > 2d){
			if(data[(int)(data.length - 2d)] == '='){
				padding = padding + 1d;
			}
		}
		quads = data.length/4d;

		/* TODO: Require init?*/
		/* Init*/
		decoded.data = new double [(int)(quads*3d - padding)];
		decoded.errorMessage = new char [0];
		decoded.success = true;

		done = false;
		for(i = 0d; i < quads && !done; i = i + 1d){
			data4 = new char [4];
			currentQuad = i*4d;
			currentTriplet = i*3d;
			if(padding > 0d && (i + 1d) == quads){
				if(padding == 2d){
					data4[0] = data[(int)(currentQuad + 0d)];
					data4[1] = data[(int)(currentQuad + 1d)];
					data4[2] = 'A';
					data4[3] = 'A';
				}else if(padding == 1d){
					data4[0] = data[(int)(currentQuad + 0d)];
					data4[1] = data[(int)(currentQuad + 1d)];
					data4[2] = data[(int)(currentQuad + 2d)];
					data4[3] = 'A';
				}

			}else{
				data4[0] = data[(int)(currentQuad + 0d)];
				data4[1] = data[(int)(currentQuad + 1d)];
				data4[2] = data[(int)(currentQuad + 2d)];
				data4[3] = data[(int)(currentQuad + 3d)];
			}
			decoded3 = decode3table(data4, table, validTable);
			if(decoded3.success){
				if(padding > 0d && i + 1d == quads){
					if(padding == 2d){
						decoded.data[(int)(currentTriplet + 0d)] = decoded3.data[0];
					}else if(padding == 1d){
						decoded.data[(int)(currentTriplet + 0d)] = decoded3.data[0];
						decoded.data[(int)(currentTriplet + 1d)] = decoded3.data[1];
					}

				}else{
					decoded.data[(int)(currentTriplet + 0d)] = decoded3.data[0];
					decoded.data[(int)(currentTriplet + 1d)] = decoded3.data[1];
					decoded.data[(int)(currentTriplet + 2d)] = decoded3.data[2];
				}
			}else{
				decoded.success = false;
				decoded.errorMessage = decoded3.errorMessage;
				done = true;
			}
			delete(decoded3);
		}

		delete(table);
		/* Unalloc 1*/
		return decoded;
	}

	public static Decoded decode3(char [] data){
		double [] table;
		boolean [] validTable;
		Decoded decoded;

		table = getRevTable();
		/* Alloc 1*/
		validTable = getValidTable();
		/* Alloc 2*/
		decoded = decode3table(data, table, validTable);

		delete(validTable);
		/* Unalloc 2*/
		delete(table);
		/* Unalloc 1*/
		return decoded;
	}

	public static Decoded decode3table(char [] data, double [] table, boolean [] validTable){
		double i, valid;
		Decoded decoded;

		decoded = new Decoded();
		/* Alloc r*/
		decoded.success = false;
		decoded.data = new double [3];
		decoded.errorMessage = "".toCharArray();

		if(data.length == 4d){
			valid = 0d;
			for(i = 0d; i < 4d; i = i + 1d){
				if(isValidBase64characterTable(data[(int)(i)], validTable)){
					valid = valid + 1d;
				}
			}
			if(valid == 4d){
				decode3NoChecksTable(decoded, data, table);
				decoded.success = true;
			}else{
				decoded.errorMessage = "Invalid character in input string.".toCharArray();
				decoded.success = false;
			}
		}else{
			decoded.errorMessage = "There must be exactly four characters in the input string.".toCharArray();
			decoded.success = false;
		}

		return decoded;
	}

	public static boolean isValidBase64characterTable(char c, boolean [] validTable){

		return validTable[(int)(c)];
	}

	public static void decode3NoChecks(Decoded decoded, char [] data){
		double [] table;

		table = getRevTable();
		decode3NoChecksTable(decoded, data, table);
	}

	public static void decode3NoChecksTable(Decoded decoded, char [] data, double [] table){
		double total, i, n, r;

		total = 0d;
		for(i = 0d; i < 4d; i = i + 1d){
			n = getNumber(data[(int)(4d - i - 1d)], table);
			total = total + n*pow(2d, i*6d);
		}

		for(i = 0d; i < 3d; i = i + 1d){
			r = total%pow(2d, 8d);
			decoded.data[(int)(3d - i - 1d)] = r;
			total = (total - r)/pow(2d, 8d);
		}
	}

	public static double getNumber(char c, double [] table){

		return table[(int)(c)];
	}

	public static Encoded encode(double [] data){
		Encoded encoded, encoded3;
		double padding, triplets, i, currentTriplet, currentQuad;
		double [] data3;
		boolean done;

		encoded = new Encoded();

		padding = 0d;
		if((data.length%3d) == 1d){
			padding = 2d;
		}
		if((data.length%3d) == 2d){
			padding = 1d;
		}
		triplets = ceil(data.length/3d);

		/* Init*/
		encoded.data = new char [(int)(triplets*4d)];
		encoded.errorMessage = new char [0];
		encoded.success = true;

		done = false;
		for(i = 0d; i < triplets && !done; i = i + 1d){
			data3 = new double [3];
			currentTriplet = i*3d;
			currentQuad = i*4d;
			if(padding > 0d && i + 1d == triplets){
				if(padding == 2d){
					data3[0] = data[(int)(currentTriplet + 0d)];
					data3[1] = 0d;
					data3[2] = 0d;
				}else if(padding == 1d){
					data3[0] = data[(int)(currentTriplet + 0d)];
					data3[1] = data[(int)(currentTriplet + 1d)];
					data3[2] = 0d;
				}

			}else{
				data3[0] = data[(int)(currentTriplet + 0d)];
				data3[1] = data[(int)(currentTriplet + 1d)];
				data3[2] = data[(int)(currentTriplet + 2d)];
			}
			encoded3 = encode3(data3);
			if(encoded3.success){
				encoded.data[(int)(currentQuad + 0d)] = encoded3.data[0];
				encoded.data[(int)(currentQuad + 1d)] = encoded3.data[1];
				if(padding > 0d && i + 1d == triplets){
					if(padding == 2d){
						encoded.data[(int)(currentQuad + 2d)] = '=';
						encoded.data[(int)(currentQuad + 3d)] = '=';
					}else if(padding == 1d){
						encoded.data[(int)(currentQuad + 2d)] = encoded3.data[2];
						encoded.data[(int)(currentQuad + 3d)] = '=';
					}

				}else{
					encoded.data[(int)(currentQuad + 2d)] = encoded3.data[2];
					encoded.data[(int)(currentQuad + 3d)] = encoded3.data[3];
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

	public static Encoded encode3(double [] data){
		Encoded encoded;
		double elementsVerified, i, e;
		boolean isWithinBounds, isWhole;

		encoded = new Encoded();
		/* Init*/
		encoded.data = new char [4];
		encoded.data[0] = 'A';
		encoded.data[1] = 'A';
		encoded.data[2] = 'A';
		encoded.data[3] = 'A';
		encoded.errorMessage = new char [0];
		encoded.success = false;

		/* Check.*/
		if(data.length == 3d){
			elementsVerified = 0d;
			for(i = 0d; i < data.length; i = i + 1d){
				e = data[(int)(i)];
				isWithinBounds = (e >= 0d) && (e < pow(2d, 8d));
				isWhole = (e%1d) == 0d;
				if(isWithinBounds && isWhole){
					elementsVerified = elementsVerified + 1d;
				}else{
					encoded.errorMessage = "Input number is too high, too low or is not a whole number.".toCharArray();
					encoded.success = false;
				}
			}
			if(elementsVerified == data.length){
				encode3NoChecks(encoded, data);
				encoded.success = true;
			}
		}else{
			encoded.errorMessage = "Input must contain 3 numbers.".toCharArray();
			encoded.success = false;
		}

		return encoded;
	}

	public static void encode3NoChecks(Encoded encoded, double [] data){
		double total, i, bit6;
		char c;

		total = 0d;
		for(i = 0d; i < data.length; i = i + 1d){
			total = total + data[(int)(data.length - i - 1d)]*pow(2d, i*8d);
		}

		for(i = 0d; i < 4d; i = i + 1d){
			bit6 = total%pow(2d, 6d);
			total = (total - bit6)/pow(2d, 6d);
			c = getCharacter(bit6);
			encoded.data[(int)(4d - i - 1d)] = c;
		}
	}

	public static char getCharacter(double bit6){
		char [] table;

		table = getTable();

		return table[(int)(bit6)];
	}

	public static char [] getTable(){

		return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".toCharArray();
	}

	public static double [] getRevTable(){
		char [] table;
		double i, max;
		double [] revTable;

		table = getTable();
		max = getMax(table) + 1d;
		revTable = new double [(int)(max + 1d)];

		for(i = 0d; i < table.length; i = i + 1d){
			revTable[(int)(table[(int)(i)])] = i;
		}

		return revTable;
	}

	public static boolean [] getValidTable(){
		char [] table;
		double max, i;
		boolean [] validTable;

		table = getTable();
		max = getMax(table) + 1d;
		validTable = new boolean [(int)(max)];

		for(i = 0d; i < max; i = i + 1d){
			validTable[(int)(i)] = isValidBase64character((char)(i));
		}

		return validTable;
	}

	public static double getMax(char [] table){
		double maxValue;
		double v;
		double i;

		maxValue = 0d;

		for(i = 0d; i < table.length; i = i + 1d){
			v = table[(int)(i)];
			maxValue = max(maxValue, v);
		}
		return maxValue;
	}

	public static boolean isValidBase64character(char c){
		char [] table;
		boolean isValid;
		double i;

		table = getTable();
		isValid = false;

		for(i = 0d; i < table.length; i = i + 1d){
			if(table[(int)(i)] == c){
				isValid = true;
			}
		}

		return isValid;
	}

	public static testresult testDecoder(){
		char [] orginal, result;
		double [] data;
		Encoded encoded;
		Decoded decoded;
		testresult atestresult;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		orginal = "Hei".toCharArray();
		data = stringToNumberArray(orginal);
		encoded = encode(data);

		decoded = decode3(encoded.data);

		result = numberArrayToString(decoded.data);

		if(!stringsEqual(orginal, result)){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!decoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
		}

		return atestresult;
	}

	public static testresult testDecoder2(){
		testresult atestresult;
		double [] original;
		Encoded encoded;
		Decoded decoded;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		original = new double [3];
		original[0] = 0d;
		original[1] = pow(2d, 4d);
		original[2] = pow(2d, 8d) - 1d;
		encoded = encode(original);

		decoded = decode3(encoded.data);

		if(!numberArraysEqual(original, decoded.data)){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!decoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
		}

		return atestresult;
	}

	public static testresult testdecoderWithPadding(){
		testresult atestresult;
		char [] original;
		double [] data;
		Encoded encoded;
		Decoded decoded;
		char [] result;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		original = "Hei12".toCharArray();
		data = stringToNumberArray(original);
		encoded = encode(data);

		decoded = decode(encoded.data);

		result = numberArrayToString(decoded.data);

		if(!stringsEqual(original, result)){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!decoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
		}

		return atestresult;
	}

	public static testresult testDecoderWithPadding2(){
		testresult atestresult;
		char [] original;
		double [] data;
		Encoded encoded;
		Decoded decoded;
		char [] result;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		original = "Hei1".toCharArray();
		data = stringToNumberArray(original);
		encoded = encode(data);

		decoded = decode(encoded.data);

		result = numberArrayToString(decoded.data);

		if(!stringsEqual(original, result)){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!decoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
		}

		return atestresult;
	}

	public static testresult testDecoderReal1(){
		testresult atestresult;
		Decoded decoded;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		decoded = decode("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g=".toCharArray());

		if(!decoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
		}

		return atestresult;
	}

	public static testresult testEncoder(){
		testresult atestresult;
		char [] hei;
		double [] data;
		Encoded encoded;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [0];

		hei = "Hei".toCharArray();

		data = stringToNumberArray(hei);

		encoded = encode3(data);

		if(!stringsEqual(encoded.data, "SGVp".toCharArray())){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!encoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
		}

		return atestresult;
	}

	public static string stringFrom(char [] src){
		string str;

		str = new string();
		str.str = src;
		return str;
	}

	public static testresult testEncoder2(){
		testresult atestresult;
		double [] data;
		Encoded encoded;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		/* TODO: Support direct instantiation.*/
		data = new double [3];
		data[0] = 0d;
		data[1] = pow(2d, 4d);
		data[2] = pow(2d, 8d) - 1d;

		encoded = encode3(data);

		if(!stringsEqual(encoded.data, "ABD/".toCharArray())){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!encoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
		}

		return atestresult;
	}

	public static testresult testEncoderWithPadding(){
		testresult atestresult;
		char [] hei;
		double [] data;
		Encoded encoded;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		hei = "Hei12".toCharArray();

		data = stringToNumberArray(hei);

		encoded = encode(data);

		if(!stringsEqual(encoded.data, "SGVpMTI=".toCharArray())){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!encoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
		}

		return atestresult;
	}

	public static testresult testEncoderWithPadding2(){
		testresult atestresult;
		char [] hei;
		double [] data;
		Encoded encoded;

		atestresult = new testresult();
		atestresult.failures = 0d;
		atestresult.errorMessages = new string [1];

		hei = "Hei1".toCharArray();

		data = stringToNumberArray(hei);

		encoded = encode(data);

		if(!stringsEqual(encoded.data, "SGVpMQ==".toCharArray())){
			atestresult.failures = atestresult.failures + 1d;
		}

		if(!encoded.success){
			atestresult.failures = atestresult.failures + 1d;
			atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
		}

		return atestresult;
	}

	public static double test(){
		double failures;
		testresult atestresult;

		failures = 0d;

		atestresult = testDecoder();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testDecoder2();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testdecoderWithPadding();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testDecoderWithPadding2();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testEncoder();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testEncoder2();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testEncoderWithPadding();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testEncoderWithPadding2();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		atestresult = testDecoderReal1();
		if(atestresult.failures != 0d){
			failures = failures + 1d;
		}

		return failures;
	}

	public static double [] stringToNumberArray(char [] string){
		double i;
		double [] array;

		array = new double [(int)(string.length)];

		for(i = 0d; i < string.length; i = i + 1d){
			array[(int)(i)] = string[(int)(i)];
		}
		return array;
	}

	public static char [] numberArrayToString(double [] array){
		double i;
		char [] string;

		string = new char [(int)(array.length)];

		for(i = 0d; i < array.length; i = i + 1d){
			string[(int)(i)] = (char)(array[(int)(i)]);
		}
		return string;
	}

	public static boolean stringsEqual(char [] data1, char [] data2){
		boolean equal;
		double nrEqual, i;

		equal = false;
		if(data1.length == data2.length){
			nrEqual = 0d;
			for(i = 0d; i < data1.length; i = i + 1d){
				if(data1[(int)(i)] == data2[(int)(i)]){
					nrEqual = nrEqual + 1d;
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

	public static boolean numberArraysEqual(double [] data1, double [] data2){
		boolean equal;
		double nrEqual, i;

		equal = false;
		if(data1.length == data2.length){
			nrEqual = 0d;
			for(i = 0d; i < data1.length; i = i + 1d){
				if(data1[(int)(i)] == data2[(int)(i)]){
					nrEqual = nrEqual + 1d;
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

  public static void delete(Object object){
    // Java has garbage collection.
  }


	public static void main(String [] args){
		System.out.println(test());
	}
}
