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

public class Attribute {

	/**
	 * @return the name
	 */
	public String getName() {
		return mName;
	}

	/**
	 * @return the prefix
	 */
	public String getPrefix() {
		return mPrefix;
	}

	/**
	 * @return the namespace
	 */
	public String getNamespace() {
		return mNamespace;
	}

	/**
	 * @return the value
	 */
	public String getValue() {
		return mValue;
	}

	/**
	 * @param name
	 *            the name to set
	 */
	public void setName(final String name) {
		mName = name;
	}

	/**
	 * @param prefix
	 *            the prefix to set
	 */
	public void setPrefix(final String prefix) {
		mPrefix = prefix;
	}

	/**
	 * @param namespace
	 *            the namespace to set
	 */
	public void setNamespace(final String namespace) {
		mNamespace = namespace;
	}

	/**
	 * @param value
	 *            the value to set
	 */
	public void setValue(final String value) {
		mValue = value;
	}

	private String mName, mPrefix, mNamespace, mValue;
}