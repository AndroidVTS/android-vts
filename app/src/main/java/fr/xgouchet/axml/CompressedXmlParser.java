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

import android.util.Log;

import org.w3c.dom.Document;

import java.io.IOException;
import java.io.InputStream;
import java.text.DecimalFormat;
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.ParserConfigurationException;

public class CompressedXmlParser {

	public static final String TAG = "CXP";

	public static final int WORD_START_DOCUMENT = 0x00080003;

	public static final int WORD_STRING_TABLE = 0x001C0001;
	public static final int WORD_RES_TABLE = 0x00080180;

	public static final int WORD_START_NS = 0x00100100;
	public static final int WORD_END_NS = 0x00100101;
	public static final int WORD_START_TAG = 0x00100102;
	public static final int WORD_END_TAG = 0x00100103;
	public static final int WORD_TEXT = 0x00100104;
	public static final int WORD_EOS = 0xFFFFFFFF;
	public static final int WORD_SIZE = 4;

	private static final int TYPE_ID_REF = 0x01000008;
	private static final int TYPE_ATTR_REF = 0x02000008;
	private static final int TYPE_STRING = 0x03000008;
	private static final int TYPE_DIMEN = 0x05000008;
	private static final int TYPE_FRACTION = 0x06000008;
	private static final int TYPE_INT = 0x10000008;
	private static final int TYPE_FLOAT = 0x04000008;

	private static final int TYPE_FLAGS = 0x11000008;
	private static final int TYPE_BOOL = 0x12000008;
	private static final int TYPE_COLOR = 0x1C000008;
	private static final int TYPE_COLOR2 = 0x1D000008;

	private static final String[] DIMEN = new String[] { "px", "dp", "sp",
			"pt", "in", "mm" };

	public CompressedXmlParser() {
		mNamespaces = new HashMap<String, String>();
	}

	/**
	 * Parses the xml data in the given file,
	 * 
	 * @param source
	 *            the source file to parse
	 * @param listener
	 *            the listener for XML events (must not be null)
	 * @throws IOException
	 *             if the input can't be read
	 */
	public void parse(final InputStream input,
			final CompressedXmlParserListener listener) throws IOException {

		if (listener == null) {
			throw new IllegalArgumentException(
					"CompressedXmlParser Listener can' be null");
		}
		mListener = listener;

		// TODO is.available may not be accurate !!!
		mData = new byte[input.available()];
		input.read(mData);
		input.close();

		// parseCompressedHeader();
		parseCompressedXml();

	}

	/**
	 * Parses the xml data in the given file,
	 * 
	 * @param source
	 *            the source file to parse
	 * @return the DOM document object
	 * 
	 * @throws IOException
	 *             if the input can't be read
	 * @throws ParserConfigurationException
	 *             if a DocumentBuilder can't be created
	 */
	public Document parseDOM(final InputStream input) throws IOException,
			ParserConfigurationException {
		CompressedXmlDomListener dom = new CompressedXmlDomListener();

		parse(input, dom);

		return dom.getDocument();
	}

	/**
	 * Each tag starts with a 32 bits word (different for start tag, end tag and
	 * end doc)
	 * 
	 */
	private void parseCompressedXml() {
		int word0;

		while (mParserOffset < mData.length) {
			word0 = getLEWord(mParserOffset);
			switch (word0) {
			case WORD_START_DOCUMENT:
				parseStartDocument();
				break;
			case WORD_STRING_TABLE:
				parseStringTable();
				break;
			case WORD_RES_TABLE:
				parseResourceTable();
				break;
			case WORD_START_NS:
				parseNamespace(true);
				break;
			case WORD_END_NS:
				parseNamespace(false);
				break;
			case WORD_START_TAG:
				parseStartTag();
				break;
			case WORD_END_TAG:
				parseEndTag();
				break;
			case WORD_TEXT:
				parseText();
				break;
			case WORD_EOS:
				mListener.endDocument();
				break;
			default:
				mParserOffset += WORD_SIZE;
				Log.w(TAG, "Unknown word 0x" + Integer.toHexString(word0)
						+ " @" + mParserOffset);
				break;
			}
		}

		mListener.endDocument();
	}

	/**
	 * A doc starts with the following 4bytes words :
	 * <ul>
	 * <li>0th word : 0x00080003</li>
	 * <li>1st word : chunk size</li>
	 * </ul>
	 */
	private void parseStartDocument() {
		mListener.startDocument();
		mParserOffset += (2 * WORD_SIZE);
	}

	/**
	 * the string table starts with the following 4bytes words :
	 * <ul>
	 * <li>0th word : 0x1c0001</li>
	 * <li>1st word : chunk size</li>
	 * <li>2nd word : number of string in the string table</li>
	 * <li>3rd word : number of styles in the string table</li>
	 * <li>4th word : ???? (0)</li>
	 * <li>5th word : Offset to String data</li>
	 * <li>6th word : Offset to style data</li>
	 * </ul>
	 */
	private void parseStringTable() {

		int chunk = getLEWord(mParserOffset + (1 * WORD_SIZE));
		mStringsCount = getLEWord(mParserOffset + (2 * WORD_SIZE));
		mStylesCount = getLEWord(mParserOffset + (3 * WORD_SIZE));
		int strOffset = mParserOffset
				+ getLEWord(mParserOffset + (5 * WORD_SIZE));
		int styleOffset = getLEWord(mParserOffset + (6 * WORD_SIZE));

		mStringsTable = new String[mStringsCount];
		int offset;
		for (int i = 0; i < mStringsCount; ++i) {
			offset = strOffset
					+ getLEWord(mParserOffset + ((i + 7) * WORD_SIZE));
			mStringsTable[i] = getStringFromStringTable(offset);
		}

		if (styleOffset > 0) {
			Log.w(TAG, "Unread styles");
			for (int i = 0; i < mStylesCount; ++i) {
				// TODO read the styles ???
			}
		}

		mParserOffset += chunk;
	}

	/**
	 * the resource ids table starts with the following 4bytes words :
	 * <ul>
	 * <li>0th word : 0x00080180</li>
	 * <li>1st word : chunk size</li>
	 * </ul>
	 */
	private void parseResourceTable() {
		int chunk = getLEWord(mParserOffset + (1 * WORD_SIZE));
		mResCount = (chunk / 4) - 2;

		mResourcesIds = new int[mResCount];
		for (int i = 0; i < mResCount; ++i) {
			mResourcesIds[i] = getLEWord(mParserOffset + ((i + 2) * WORD_SIZE));
		}

		mParserOffset += chunk;
	}

	/**
	 * A namespace tag contains the following 4bytes words :
	 * <ul>
	 * <li>0th word : 0x00100100 = Start NS / 0x00100101 = end NS</li>
	 * <li>1st word : chunk size</li>
	 * <li>2nd word : line this tag appeared</li>
	 * <li>3rd word : ??? (always 0xFFFFFF)</li>
	 * <li>4th word : index of namespace prefix in StringIndexTable</li>
	 * <li>5th word : index of namespace uri in StringIndexTable</li>
	 * </ul>
	 */
	private void parseNamespace(boolean start) {
		final int prefixIdx = getLEWord(mParserOffset + (4 * WORD_SIZE));
		final int uriIdx = getLEWord(mParserOffset + (5 * WORD_SIZE));

		final String uri = getString(uriIdx);
		final String prefix = getString(prefixIdx);

		if (start) {
			mListener.startPrefixMapping(prefix, uri);
			mNamespaces.put(uri, prefix);
		} else {
			mListener.endPrefixMapping(prefix, uri);
			mNamespaces.remove(uri);
		}

		// Offset to first tag
		mParserOffset += (6 * WORD_SIZE);
	}

	/**
	 * A start tag will start with the following 4bytes words :
	 * <ul>
	 * <li>0th word : 0x00100102 = Start_Tag</li>
	 * <li>1st word : chunk size</li>
	 * <li>2nd word : line this tag appeared in the original file</li>
	 * <li>3rd word : ??? (always 0xFFFFFF)</li>
	 * <li>4th word : index of namespace uri in StringIndexTable, or 0xFFFFFFFF
	 * for default NS</li>
	 * <li>5th word : index of element name in StringIndexTable</li>
	 * <li>6th word : ???</li>
	 * <li>7th word : number of attributes following the start tag</li>
	 * <li>8th word : ??? (0)</li>
	 * </ul>
	 * 
	 */
	private void parseStartTag() {
		// get tag info
		final int uriIdx = getLEWord(mParserOffset + (4 * WORD_SIZE));
		final int nameIdx = getLEWord(mParserOffset + (5 * WORD_SIZE));
		final int attrCount = getLEWord(mParserOffset + (7 * WORD_SIZE));

		final String name = getString(nameIdx);
		String uri, qname;
		if (uriIdx == 0xFFFFFFFF) {
			uri = "";
			qname = name;
		} else {
			uri = getString(uriIdx);
			if (mNamespaces.containsKey(uri)) {
				qname = mNamespaces.get(uri) + ':' + name;
			} else {
				qname = name;
			}
		}

		// offset to start of attributes
		mParserOffset += (9 * WORD_SIZE);

		final Attribute[] attrs = new Attribute[attrCount]; // NOPMD
		for (int a = 0; a < attrCount; a++) {
			attrs[a] = parseAttribute(); // NOPMD

			// offset to next attribute or tag
			mParserOffset += (5 * 4);
		}

		mListener.startElement(uri, name, qname, attrs);
	}

	/**
	 * An attribute will have the following 4bytes words :
	 * <ul>
	 * <li>0th word : index of namespace uri in StringIndexTable, or 0xFFFFFFFF
	 * for default NS</li>
	 * <li>1st word : index of attribute name in StringIndexTable</li>
	 * <li>2nd word : index of attribute value, or 0xFFFFFFFF if value is a
	 * typed value</li>
	 * <li>3rd word : value type</li>
	 * <li>4th word : resource id value</li>
	 * </ul>
	 */
	private Attribute parseAttribute() {
		final int attrNSIdx = getLEWord(mParserOffset);
		final int attrNameIdx = getLEWord(mParserOffset + (1 * WORD_SIZE));
		final int attrValueIdx = getLEWord(mParserOffset + (2 * WORD_SIZE));
		final int attrType = getLEWord(mParserOffset + (3 * WORD_SIZE));
		final int attrData = getLEWord(mParserOffset + (4 * WORD_SIZE));

		final Attribute attr = new Attribute();
		attr.setName(getString(attrNameIdx));

		if (attrNSIdx == 0xFFFFFFFF) {
			attr.setNamespace(null);
			attr.setPrefix(null);
		} else {
			String uri = getString(attrNSIdx);
			if (mNamespaces.containsKey(uri)) {
				attr.setNamespace(uri);
				attr.setPrefix(mNamespaces.get(uri));
			}
		}

		if (attrValueIdx == 0xFFFFFFFF) {
			attr.setValue(getAttributeValue(attrType, attrData));
		} else {
			attr.setValue(getString(attrValueIdx));
		}

		return attr;

	}

	/**
	 * A text will start with the following 4bytes word :
	 * <ul>
	 * <li>0th word : 0x00100104 = Text</li>
	 * <li>1st word : chunk size</li>
	 * <li>2nd word : line this element appeared in the original document</li>
	 * <li>3rd word : ??? (always 0xFFFFFFFF)</li>
	 * <li>4rd word : string index in string table</li>
	 * <li>5rd word : ??? (always 8)</li>
	 * <li>6rd word : ??? (always 0)</li>
	 * </ul>
	 * 
	 */
	private void parseText() {
		// get tag infos
		final int strIndex = getLEWord(mParserOffset + (4 * WORD_SIZE));

		String data = getString(strIndex);
		mListener.characterData(data);

		// offset to next node
		mParserOffset += (7 * WORD_SIZE);
	}

	/**
	 * EndTag contains the following 4bytes words :
	 * <ul>
	 * <li>0th word : 0x00100103 = End_Tag</li>
	 * <li>1st word : chunk size</li>
	 * <li>2nd word : line this tag appeared in the original file</li>
	 * <li>3rd word : ??? (always 0xFFFFFFFF)</li>
	 * <li>4th word : index of namespace name in StringIndexTable, or 0xFFFFFFFF
	 * for default NS</li>
	 * <li>5th word : index of element name in StringIndexTable</li>
	 * </ul>
	 */
	private void parseEndTag() {
		// get tag info
		final int uriIdx = getLEWord(mParserOffset + (4 * WORD_SIZE));
		final int nameIdx = getLEWord(mParserOffset + (5 * WORD_SIZE));

		final String name = getString(nameIdx);
		String uri;
		if (uriIdx == 0xFFFFFFFF) {
			uri = "";
		} else {
			uri = getString(uriIdx);
		}

		mListener.endElement(uri, name, null);

		// offset to start of next tag
		mParserOffset += (6 * WORD_SIZE);
	}

	/**
	 * @param index
	 *            the index of the string in the StringIndexTable
	 * @return the string
	 */
	private String getString(final int index) {
		String res;
		if ((index >= 0) && (index < mStringsCount)) {
			res = mStringsTable[index];
		} else {
			res = null; // NOPMD
		}

		return res;
	}

	/**
	 * @param offset
	 *            offset of the beginning of the string inside the StringTable
	 *            (and not the whole data array)
	 * @return the String
	 */
	private String getStringFromStringTable(final int offset) {
		int strLength;
		byte chars[];
		if (mData[offset + 1] == mData[offset]) {
			strLength = mData[offset];
			chars = new byte[strLength];// NOPMD
			for (int i = 0; i < strLength; i++) {
				chars[i] = mData[offset + 2 + i]; // NOPMD
			}
		} else {

			strLength = ((mData[offset + 1] << 8) & 0xFF00)
					| (mData[offset] & 0xFF);
			chars = new byte[strLength]; // NOPMD
			for (int i = 0; i < strLength; i++) {
				chars[i] = mData[offset + 2 + (i * 2)]; // NOPMD
			}

		}
		return new String(chars);
	}

	/**
	 * @param arr
	 *            the byte array to read
	 * @param off
	 *            the offset of the word to read
	 * @return value of a Little Endian 32 bit word from the byte arrayat offset
	 *         off.
	 */
	private int getLEWord(final int off) {
		return ((mData[off + 3] << 24) & 0xff000000)
				| ((mData[off + 2] << 16) & 0x00ff0000)
				| ((mData[off + 1] << 8) & 0x0000ff00)
				| ((mData[off + 0] << 0) & 0x000000ff);
	}

	/**
	 * @param word
	 *            a word read in an attribute data
	 * @return the typed value
	 */
	private String getAttributeValue(final int type, final int data) {
		String res;

		switch (type) {
		case TYPE_STRING:
			res = getString(data);
			break;
		case TYPE_DIMEN:
			res = Integer.toString(data >> 8) + DIMEN[data & 0xFF];
			break;
		case TYPE_FRACTION:
			double fracValue = (((double) data) / ((double) 0x7FFFFFFF));
			// res = String.format("%.2f%%", fracValue);
			res = new DecimalFormat("#.##%").format(fracValue);
			break;
		case TYPE_FLOAT:
			res = Float.toString(Float.intBitsToFloat(data));
			break;
		case TYPE_INT:
		case TYPE_FLAGS:
			res = Integer.toString(data);
			break;
		case TYPE_BOOL:
			res = Boolean.toString(data != 0);
			break;
		case TYPE_COLOR:
		case TYPE_COLOR2:
			res = String.format("#%08X", data);
			break;
		case TYPE_ID_REF:
			res = String.format("@id/0x%08X", data);
			break;
		case TYPE_ATTR_REF:
			res = String.format("?id/0x%08X", data);
			break;
		default:
			Log.w(TAG, "(type=" + Integer.toHexString(type) + ") : " + data
					+ " (0x" + Integer.toHexString(data) + ") @"
					+ mParserOffset);
			res = String.format("%08X/0x%08X", type, data);
			break;
		}

		return res;
	}

	// Data
	private CompressedXmlParserListener mListener;

	// Internal
	private Map<String, String> mNamespaces;
	private byte[] mData;

	private String[] mStringsTable;
	private int[] mResourcesIds;
	private int mStringsCount, mStylesCount, mResCount;
	private int mParserOffset;

}
