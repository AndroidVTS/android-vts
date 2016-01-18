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

public interface CompressedXmlParserListener {

	/**
	 * Receive notification of the beginning of a document.
	 */
	void startDocument();

	/**
	 * Receive notification of the end of a document.
	 */
	void endDocument();

	/**
	 * Begin the scope of a prefix-URI Namespace mapping.
	 * 
	 * @param prefix
	 *            the Namespace prefix being declared. An empty string is used
	 *            for the default element namespace, which has no prefix.
	 * @param uri
	 *            the Namespace URI the prefix is mapped to
	 */
	void startPrefixMapping(String prefix, String uri);

	/**
	 * End the scope of a prefix-URI mapping.
	 * 
	 * @param prefix
	 *            the prefix that was being mapped. This is the empty string
	 *            when a default mapping scope ends.
	 * @param uri
	 *            the Namespace URI the prefix is mapped to
	 */
	void endPrefixMapping(String prefix, String uri);

	/**
	 * Receive notification of the beginning of an element.
	 * 
	 * @param uri
	 *            the Namespace URI, or the empty string if the element has no
	 *            Namespace URI or if Namespace processing is not being
	 *            performed
	 * @param localName
	 *            the local name (without prefix), or the empty string if
	 *            Namespace processing is not being performed
	 * @param qName
	 *            the qualified name (with prefix), or the empty string if
	 *            qualified names are not available
	 * @param atts
	 *            the attributes attached to the element. If there are no
	 *            attributes, it shall be an empty Attributes object. The value
	 *            of this object after startElement returns is undefined
	 */
	void startElement(String uri, String localName, String qName,
					  Attribute[] atts);

	/**
	 * Receive notification of the end of an element.
	 * 
	 * @param uri
	 *            the Namespace URI, or the empty string if the element has no
	 *            Namespace URI or if Namespace processing is not being
	 *            performed
	 * @param localName
	 *            the local name (without prefix), or the empty string if
	 *            Namespace processing is not being performed
	 * @param qName
	 *            the qualified XML name (with prefix), or the empty string if
	 *            qualified names are not available
	 */
	void endElement(String uri, String localName, String qName);

	/**
	 * Receive notification of text.
	 * 
	 * @param data
	 *            the text data
	 */
	void text(String data);

	/**
	 * Receive notification of character data (in a <![CDATA[ ]]> block).
	 * 
	 * @param data
	 *            the text data
	 */
	void characterData(String data);

	/**
	 * Receive notification of a processing instruction.
	 * 
	 * @param target
	 *            the processing instruction target
	 * @param data
	 *            the processing instruction data, or null if none was supplied.
	 *            The data does not include any whitespace separating it from
	 *            the target
	 * @throws org.xml.sax.SAXException
	 *             any SAX exception, possibly wrapping another exception
	 */
	void processingInstruction(String target, String data);
}
