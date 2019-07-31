/*
* Copyright (C) 2012 by Xavier GOUCHET (http://xgouchet.fr, android@xgouchet.fr) MIT Licence / Expat
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
* associated documentation files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute,
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS ( XAVIER GOUCHET ) BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
* IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

package fr.xgouchet.axml;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

public final class CompressedXmlUtils {

	public static boolean isCompressedXml(final InputStream input) {
		boolean result;

		try {
			final byte[] header = new byte[4];
			input.read(header, 0, 4);

			result = true;
			result &= (header[0] == 0x03);
			result &= (header[1] == 0x00);
			result &= (header[2] == 0x08);
			result &= (header[3] == 0x00);

		} catch (Exception e) {
			result = false;
		} finally {
			try {
				input.close();
			} catch (Exception e) {
				// ignore this exception
			}
		}

		return result;
	}

	public static boolean isCompressedXml(final File source) {
		boolean result;

		try {
			final InputStream input = new FileInputStream(source.getPath());
			result = isCompressedXml(input);
		} catch (Exception e) {
			result = false;
		}

		return result;
	}

	private CompressedXmlUtils() {
	}
}
