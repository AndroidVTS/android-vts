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

import android.text.TextUtils;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import java.util.Stack;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

public class CompressedXmlDomListener implements CompressedXmlParserListener {

	/**
	 * @throws ParserConfigurationException
	 *             if a DocumentBuilder can't be created
	 */
	public CompressedXmlDomListener() throws ParserConfigurationException {
		mBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
		mStack = new Stack<>();
	}

	public void startDocument() {
		mDocument = mBuilder.newDocument();
		mStack.push(mDocument);
	}

	public void endDocument() {
	}

	public void startPrefixMapping(String prefix, String uri) {
	}

	public void endPrefixMapping(String prefix, String uri) {
	}

	public void startElement(final String uri, final String localName,
			final String qName, final Attribute[] attrs) {
		Element elt;

		// create elt
		if (TextUtils.isEmpty(uri)) {
			elt = mDocument.createElement(localName);
		} else {
			elt = mDocument.createElementNS(uri, qName);
		}

		// add attrs
		for (Attribute attr : attrs) {
			if (TextUtils.isEmpty(attr.getNamespace())) {
				elt.setAttribute(attr.getName(), attr.getValue());
			} else {
				elt.setAttributeNS(attr.getNamespace(), attr.getPrefix() + ':'
						+ attr.getName(), attr.getValue());
			}
		}

		// handle stack
		mStack.peek().appendChild(elt);
		mStack.push(elt);
	}

	public void endElement(String uri, String localName, String qName) {
		mStack.pop();
	}

	public void characterData(String data) {
		mStack.peek().appendChild(mDocument.createCDATASection(data));
	}

	public void text(String data) {
		mStack.peek().appendChild(mDocument.createTextNode(data));
	}

	public void processingInstruction(String target, String data) {
	}

	/**
	 * 
	 */
	public Document getDocument() {
		return mDocument;
	}

	private Stack<Node> mStack;
	private Document mDocument;
	private final DocumentBuilder mBuilder;
}
