Imports System
Imports System.Math

Public Structure Decoded
	Public data As Double ()
	Public success As Boolean
	Public errorMessage As Char ()
End Structure

Public Structure Encoded
	Public data As Char ()
	Public success As Boolean
	Public errorMessage As Char ()
End Structure

Public Structure stringxx
	Public str As Char ()
End Structure

Public Structure testresult
	Public failures As Double
	Public errorMessages As stringxx ()
End Structure

Module base64
	Public Function decode(ByRef data As Char ()) As Decoded
		Dim done As Boolean
		Dim data4 As Char ()
		Dim quads, i, currentQuad, currentTriplet, padding As Double
		Dim decoded3, decoded As Decoded
		Dim table As Double ()
		Dim validTable As Boolean ()

		decoded = New Decoded()
		table = getRevTable()
		' Alloc 1
		validTable = getValidTable()
		' Alloc 3
		padding = 0
		If data.Length > 1
			If data(data.Length - 1) = "="C
				padding = padding + 1
			End If
		End If
		' TODO: Evaluate && left to right and stop if false?
		If data.Length > 2
			If data(data.Length - 2) = "="C
				padding = padding + 1
			End If
		End If
		quads = data.Length/4

		' TODO: Require init?
		' Init
		decoded.data = New Double (quads*3 - padding - 1){}
		decoded.errorMessage = New Char (0 - 1){}
		decoded.success = true

		done = false
		i = 0
		While i < quads And Not done
			data4 = New Char (4 - 1){}
			currentQuad = i*4
			currentTriplet = i*3
			If padding > 0 And (i + 1) = quads
				If padding = 2
					data4(0) = data(currentQuad + 0)
					data4(1) = data(currentQuad + 1)
					data4(2) = "A"C
					data4(3) = "A"C
				ElseIf padding = 1
					data4(0) = data(currentQuad + 0)
					data4(1) = data(currentQuad + 1)
					data4(2) = data(currentQuad + 2)
					data4(3) = "A"C
				End If

			Else
				data4(0) = data(currentQuad + 0)
				data4(1) = data(currentQuad + 1)
				data4(2) = data(currentQuad + 2)
				data4(3) = data(currentQuad + 3)
			End If
			decoded3 = decode3table(data4, table, validTable)
			If decoded3.success
				If padding > 0 And i + 1 = quads
					If padding = 2
						decoded.data(currentTriplet + 0) = decoded3.data(0)
					ElseIf padding = 1
						decoded.data(currentTriplet + 0) = decoded3.data(0)
						decoded.data(currentTriplet + 1) = decoded3.data(1)
					End If

				Else
					decoded.data(currentTriplet + 0) = decoded3.data(0)
					decoded.data(currentTriplet + 1) = decoded3.data(1)
					decoded.data(currentTriplet + 2) = decoded3.data(2)
				End If
			Else
				decoded.success = false
				decoded.errorMessage = decoded3.errorMessage
				done = true
			End If
			decoded3 = Nothing
			i = i + 1
		End While

		Erase table 
		' Unalloc 1
		Return decoded
	End Function


	Public Function decode3(ByRef data As Char ()) As Decoded
		Dim table As Double ()
		Dim validTable As Boolean ()
		Dim decoded As Decoded

		table = getRevTable()
		' Alloc 1
		validTable = getValidTable()
		' Alloc 2
		decoded = decode3table(data, table, validTable)

		Erase validTable 
		' Unalloc 2
		Erase table 
		' Unalloc 1
		Return decoded
	End Function


	Public Function decode3table(ByRef data As Char (), ByRef table As Double (), ByRef validTable As Boolean ()) As Decoded
		Dim i, valid As Double
		Dim decoded As Decoded

		decoded = New Decoded()
		' Alloc r
		decoded.success = false
		decoded.data = New Double (3 - 1){}
		decoded.errorMessage = "".ToCharArray()

		If data.Length = 4
			valid = 0
			i = 0
			While i < 4
				If isValidBase64characterTable(data(i), validTable)
					valid = valid + 1
				End If
				i = i + 1
			End While
			If valid = 4
				Call decode3NoChecksTable(decoded, data, table)
				decoded.success = true
			Else
				decoded.errorMessage = "Invalid character in input string.".ToCharArray()
				decoded.success = false
			End If
		Else
			decoded.errorMessage = "There must be exactly four characters in the input string.".ToCharArray()
			decoded.success = false
		End If

		Return decoded
	End Function


	Public Function isValidBase64characterTable(c As Char, ByRef validTable As Boolean ()) As Boolean

		Return validTable(Convert.ToByte(c))
	End Function


	Public Sub decode3NoChecks(ByRef decoded As Decoded, ByRef data As Char ())
		Dim table As Double ()

		table = getRevTable()
		Call decode3NoChecksTable(decoded, data, table)
	End Sub


	Public Sub decode3NoChecksTable(ByRef decoded As Decoded, ByRef data As Char (), ByRef table As Double ())
		Dim total, i, n, r As Double

		total = 0
		i = 0
		While i < 4
			n = getNumber(data(4 - i - 1), table)
			total = total + n*2 ^ (i*6)
			i = i + 1
		End While

		i = 0
		While i < 3
			r = total Mod 2 ^ 8
			decoded.data(3 - i - 1) = r
			total = (total - r)/2 ^ 8
			i = i + 1
		End While
	End Sub


	Public Function getNumber(c As Char, ByRef table As Double ()) As Double

		Return table(Convert.ToByte(c))
	End Function


	Public Function encode(ByRef data As Double ()) As Encoded
		Dim encoded, encoded3 As Encoded
		Dim padding, triplets, i, currentTriplet, currentQuad As Double
		Dim data3 As Double ()
		Dim done As Boolean

		encoded = New Encoded()

		padding = 0
		If (data.Length Mod 3) = 1
			padding = 2
		End If
		If (data.Length Mod 3) = 2
			padding = 1
		End If
		triplets = Ceiling(data.Length/3)

		' Init
		encoded.data = New Char (triplets*4 - 1){}
		encoded.errorMessage = New Char (0 - 1){}
		encoded.success = true

		done = false
		i = 0
		While i < triplets And Not done
			data3 = New Double (3 - 1){}
			currentTriplet = i*3
			currentQuad = i*4
			If padding > 0 And i + 1 = triplets
				If padding = 2
					data3(0) = data(currentTriplet + 0)
					data3(1) = 0
					data3(2) = 0
				ElseIf padding = 1
					data3(0) = data(currentTriplet + 0)
					data3(1) = data(currentTriplet + 1)
					data3(2) = 0
				End If

			Else
				data3(0) = data(currentTriplet + 0)
				data3(1) = data(currentTriplet + 1)
				data3(2) = data(currentTriplet + 2)
			End If
			encoded3 = encode3(data3)
			If encoded3.success
				encoded.data(currentQuad + 0) = encoded3.data(0)
				encoded.data(currentQuad + 1) = encoded3.data(1)
				If padding > 0 And i + 1 = triplets
					If padding = 2
						encoded.data(currentQuad + 2) = "="C
						encoded.data(currentQuad + 3) = "="C
					ElseIf padding = 1
						encoded.data(currentQuad + 2) = encoded3.data(2)
						encoded.data(currentQuad + 3) = "="C
					End If

				Else
					encoded.data(currentQuad + 2) = encoded3.data(2)
					encoded.data(currentQuad + 3) = encoded3.data(3)
				End If
			Else
				encoded.success = false
				encoded.errorMessage = encoded3.errorMessage
				done = true
			End If
			encoded3 = Nothing
			i = i + 1
		End While

		Return encoded
	End Function


	Public Function encode3(ByRef data As Double ()) As Encoded
		Dim encoded As Encoded
		Dim elementsVerified, i, e As Double
		Dim isWithinBounds, isWhole As Boolean

		encoded = New Encoded()
		' Init
		encoded.data = New Char (4 - 1){}
		encoded.data(0) = "A"C
		encoded.data(1) = "A"C
		encoded.data(2) = "A"C
		encoded.data(3) = "A"C
		encoded.errorMessage = New Char (0 - 1){}
		encoded.success = false

		' Check.
		If data.Length = 3
			elementsVerified = 0
			i = 0
			While i < data.Length
				e = data(i)
				isWithinBounds = (e >= 0) And (e < 2 ^ 8)
				isWhole = (e Mod 1) = 0
				If isWithinBounds And isWhole
					elementsVerified = elementsVerified + 1
				Else
					encoded.errorMessage = "Input number is too high, too low or is not a whole number.".ToCharArray()
					encoded.success = false
				End If
				i = i + 1
			End While
			If elementsVerified = data.Length
				Call encode3NoChecks(encoded, data)
				encoded.success = true
			End If
		Else
			encoded.errorMessage = "Input must contain 3 numbers.".ToCharArray()
			encoded.success = false
		End If

		Return encoded
	End Function


	Public Sub encode3NoChecks(ByRef encoded As Encoded, ByRef data As Double ())
		Dim total, i, bit6 As Double
		Dim c As Char

		total = 0
		i = 0
		While i < data.Length
			total = total + data(data.Length - i - 1)*2 ^ (i*8)
			i = i + 1
		End While

		i = 0
		While i < 4
			bit6 = total Mod 2 ^ 6
			total = (total - bit6)/2 ^ 6
			c = getCharacter(bit6)
			encoded.data(4 - i - 1) = c
			i = i + 1
		End While
	End Sub


	Public Function getCharacter(bit6 As Double) As Char
		Dim table As Char ()

		table = getTable()

		Return table(bit6)
	End Function


	Public Function getTable() As Char ()

		Return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".ToCharArray()
	End Function


	Public Function getRevTable() As Double ()
		Dim table As Char ()
		Dim i, maxx As Double
		Dim revTable As Double ()

		table = getTable()
		maxx = getMax(table) + 1
		revTable = New Double (maxx + 1 - 1){}

		i = 0
		While i < table.Length
			revTable(Convert.ToByte(table(i))) = i
			i = i + 1
		End While

		Return revTable
	End Function


	Public Function getValidTable() As Boolean ()
		Dim table As Char ()
		Dim maxx, i As Double
		Dim validTable As Boolean ()

		table = getTable()
		maxx = getMax(table) + 1
		validTable = New Boolean (maxx - 1){}

		i = 0
		While i < maxx
			validTable(i) = isValidBase64character(Convert.ToChar(Convert.ToInt64(i)))
			i = i + 1
		End While

		Return validTable
	End Function


	Public Function getMax(ByRef table As Char ()) As Double
		Dim maxValue As Double
		Dim v As Double
		Dim i As Double

		maxValue = 0

		i = 0
		While i < table.Length
			v = Convert.ToByte(table(i))
			maxValue = Max(maxValue, v)
			i = i + 1
		End While
		Return maxValue
	End Function


	Public Function isValidBase64character(c As Char) As Boolean
		Dim table As Char ()
		Dim isValid As Boolean
		Dim i As Double

		table = getTable()
		isValid = false

		i = 0
		While i < table.Length
			If table(i) = c
				isValid = true
			End If
			i = i + 1
		End While

		Return isValid
	End Function


	Public Function testDecoder() As testresult
		Dim orginal, result As Char ()
		Dim data As Double ()
		Dim encoded As Encoded
		Dim decoded As Decoded
		Dim atestresult As testresult

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		orginal = "Hei".ToCharArray()
		data = stringToNumberArray(orginal)
		encoded = encode(data)

		decoded = decode3(encoded.data)

		result = numberArrayToString(decoded.data)

		If Not stringsEqual(orginal, result)
			atestresult.failures = atestresult.failures + 1
		End If

		If Not decoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(decoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function testDecoder2() As testresult
		Dim atestresult As testresult
		Dim original As Double ()
		Dim encoded As Encoded
		Dim decoded As Decoded

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		original = New Double (3 - 1){}
		original(0) = 0
		original(1) = 2 ^ 4
		original(2) = 2 ^ 8 - 1
		encoded = encode(original)

		decoded = decode3(encoded.data)

		If Not numberArraysEqual(original, decoded.data)
			atestresult.failures = atestresult.failures + 1
		End If

		If Not decoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(decoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function testdecoderWithPadding() As testresult
		Dim atestresult As testresult
		Dim original As Char ()
		Dim data As Double ()
		Dim encoded As Encoded
		Dim decoded As Decoded
		Dim result As Char ()

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		original = "Hei12".ToCharArray()
		data = stringToNumberArray(original)
		encoded = encode(data)

		decoded = decode(encoded.data)

		result = numberArrayToString(decoded.data)

		If Not stringsEqual(original, result)
			atestresult.failures = atestresult.failures + 1
		End If

		If Not decoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(decoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function testDecoderWithPadding2() As testresult
		Dim atestresult As testresult
		Dim original As Char ()
		Dim data As Double ()
		Dim encoded As Encoded
		Dim decoded As Decoded
		Dim result As Char ()

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		original = "Hei1".ToCharArray()
		data = stringToNumberArray(original)
		encoded = encode(data)

		decoded = decode(encoded.data)

		result = numberArrayToString(decoded.data)

		If Not stringsEqual(original, result)
			atestresult.failures = atestresult.failures + 1
		End If

		If Not decoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(decoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function testDecoderReal1() As testresult
		Dim atestresult As testresult
		Dim decoded As Decoded

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		decoded = decode("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g=".ToCharArray())

		If Not decoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(decoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function testEncoder() As testresult
		Dim atestresult As testresult
		Dim hei As Char ()
		Dim data As Double ()
		Dim encoded As Encoded

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (0 - 1){}

		hei = "Hei".ToCharArray()

		data = stringToNumberArray(hei)

		encoded = encode3(data)

		If Not stringsEqual(encoded.data, "SGVp".ToCharArray())
			atestresult.failures = atestresult.failures + 1
		End If

		If Not encoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(encoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function stringFrom(ByRef src As Char ()) As stringxx
		Dim str As stringxx

		str = New stringxx()
		str.str = src
		Return str
	End Function


	Public Function testEncoder2() As testresult
		Dim atestresult As testresult
		Dim data As Double ()
		Dim encoded As Encoded

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		' TODO: Support direct instantiation.
		data = New Double (3 - 1){}
		data(0) = 0
		data(1) = 2 ^ 4
		data(2) = 2 ^ 8 - 1

		encoded = encode3(data)

		If Not stringsEqual(encoded.data, "ABD/".ToCharArray())
			atestresult.failures = atestresult.failures + 1
		End If

		If Not encoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(encoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function testEncoderWithPadding() As testresult
		Dim atestresult As testresult
		Dim hei As Char ()
		Dim data As Double ()
		Dim encoded As Encoded

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		hei = "Hei12".ToCharArray()

		data = stringToNumberArray(hei)

		encoded = encode(data)

		If Not stringsEqual(encoded.data, "SGVpMTI=".ToCharArray())
			atestresult.failures = atestresult.failures + 1
		End If

		If Not encoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(encoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function testEncoderWithPadding2() As testresult
		Dim atestresult As testresult
		Dim hei As Char ()
		Dim data As Double ()
		Dim encoded As Encoded

		atestresult = New testresult()
		atestresult.failures = 0
		atestresult.errorMessages = New stringxx (1 - 1){}

		hei = "Hei1".ToCharArray()

		data = stringToNumberArray(hei)

		encoded = encode(data)

		If Not stringsEqual(encoded.data, "SGVpMQ==".ToCharArray())
			atestresult.failures = atestresult.failures + 1
		End If

		If Not encoded.success
			atestresult.failures = atestresult.failures + 1
			atestresult.errorMessages(0) = stringFrom(encoded.errorMessage)
		End If

		Return atestresult
	End Function


	Public Function test() As Double
		Dim failures As Double
		Dim atestresult As testresult

		failures = 0

		atestresult = testDecoder()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testDecoder2()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testdecoderWithPadding()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testDecoderWithPadding2()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testEncoder()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testEncoder2()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testEncoderWithPadding()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testEncoderWithPadding2()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		atestresult = testDecoderReal1()
		If atestresult.failures <> 0
			failures = failures + 1
		End If

		Return failures
	End Function


	Public Function stringToNumberArray(ByRef stringxx As Char ()) As Double ()
		Dim i As Double
		Dim arrayx As Double ()

		arrayx = New Double (stringxx.Length - 1){}

		i = 0
		While i < stringxx.Length
			arrayx(i) = Convert.ToByte(stringxx(i))
			i = i + 1
		End While
		Return arrayx
	End Function


	Public Function numberArrayToString(ByRef arrayx As Double ()) As Char ()
		Dim i As Double
		Dim stringxx As Char ()

		stringxx = New Char (arrayx.Length - 1){}

		i = 0
		While i < arrayx.Length
			stringxx(i) = Convert.ToChar(Convert.ToInt64(arrayx(i)))
			i = i + 1
		End While
		Return stringxx
	End Function


	Public Function stringsEqual(ByRef data1 As Char (), ByRef data2 As Char ()) As Boolean
		Dim equal As Boolean
		Dim nrEqual, i As Double

		equal = false
		If data1.Length = data2.Length
			nrEqual = 0
			i = 0
			While i < data1.Length
				If data1(i) = data2(i)
					nrEqual = nrEqual + 1
				End If
				i = i + 1
			End While
			If nrEqual = data1.Length
				equal = true
			End If
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Function numberArraysEqual(ByRef data1 As Double (), ByRef data2 As Double ()) As Boolean
		Dim equal As Boolean
		Dim nrEqual, i As Double

		equal = false
		If data1.Length = data2.Length
			nrEqual = 0
			i = 0
			While i < data1.Length
				If data1(i) = data2(i)
					nrEqual = nrEqual + 1
				End If
				i = i + 1
			End While
			If nrEqual = data1.Length
				equal = true
			End If
		Else
			equal = false
		End If

		Return equal
	End Function


End Module


Module base64TestBundle
	Sub Main(args As String())
		Console.WriteLine(test())
	End Sub
End Module
