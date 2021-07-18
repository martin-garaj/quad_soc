//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Diagnostic system of photo-voltaic panels           //
//                                                                //
//================================================================//
#ifndef XML_PARSER_HPP_
#define XML_PARSER_HPP_

// XML write
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/XML/XMLWriter.h"
#include <iostream>
using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::Text;
using Poco::XML::AutoPtr;
using Poco::XML::DOMWriter;
using Poco::XML::XMLWriter;
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/Exception.h>
using Poco::XML::DOMParser;
using Poco::XML::InputSource;
using Poco::XML::Document;
using Poco::XML::Node;
using Poco::XML::NodeList;
using Poco::XML::AutoPtr;
using Poco::Exception;

#include <iomanip> // setprecision
#include <sstream> // stringstream

#include <iostream>
#include <cstdlib>	// hex conversion
#include <string>	// std::string

//================================================================================================//
//                                                                								  //
//	CLASS	: XML_parser is a base class for using DOM parser. It is inherited by all classes	  //
//			  that parse XML files.  															  //
//                                                                								  //
//================================================================================================//
class XML_parser{
public:
	XML_parser(){};
	~XML_parser(){};

protected:
	// open file with DOM parser
	int openXML(DOMParser* parser, InputSource* src, Document** document);
	// parentElement based
	int getNodesAmount(const Element& parentElement, const std::string& name);
	NodeList* getNodeList(const Element& parentElement, const std::string& name);
	std::string parseChildElementText(const Element& parentElement, const std::string& name);
	// element based
	Element* getChildElement(const Element& parentElement, const std::string& name);
	std::string parseElementText(const Element& givenElement);
	std::string parseElementAttribute(const Element& givenElement, const std::string& name);
	// node based
	Element* nodeToElement(Node * node);

	// string manipulations
	unsigned long stringToHexU(const std::string& string);
	long stringToHex(const std::string& string);
	int extractString(const std::string& string, const std::string& start_node, const std::string& end_node, std::string * ptr_result);
};


int XML_parser::openXML(DOMParser* parser, InputSource* src, Document** document)
{
	try
	{

		*document = parser->parse(src);

		return RETURN_SUCCESS;
	}
	catch (...)
	{
		std::cerr << "XML_parser::openXML() ERROR" << std::endl;
		return RETURN_ERROR;
	}

	return RETURN_ERROR;

};


int XML_parser::getNodesAmount(const Element& parentElement, const std::string& name)
{
/*	param  	:	const Element& parentElement
 * 					- reference to parent element
 * 				const std::string& name
 * 					- name of element
 *
 * 	return 	:	int
 * 					- number of elements with provided "name"
 *
 * 	e.g.		<document>
 * 					<element>
 * 					<element>
 * 					<element>
 *
 * 	calling getNodesAmount(<document>, "element") returns 3
 */
	NodeList* nodes = parentElement.getElementsByTagName(name);

	return nodes->length();
};


NodeList* XML_parser::getNodeList(const Element& parentElement, const std::string& name)
{
/* 	param  	:	const Element& parentElement
 * 					- parent element
 * 				const std::string& name
 * 					- name of element
 *
 * 	return 	:	NodeList*
 * 					- pointer to list of nodes
 *
 * 	e.g.		<document>
 * 					<element>	1st
 * 					<element>	2nd
 * 					<element>	3rd
 *
 * 	calling getNodeList(<document>, "element") returns pointer to NodeList
 * 		NodeList->item(0) => <element>	1st
 * 		NodeList->item(1) => <element>	2nd
 * 		NodeList->item(2) => <element>	3rd
 */
	NodeList* nodes = parentElement.getElementsByTagName(name);
	return nodes;
}



std::string XML_parser::parseChildElementText(const Element& parentElement, const std::string& name)
{
/* 	param  	:	const Element& parentElement
 * 					- parent element
 * 				const std::string& name
 * 					- name of element
 *
 * 	return 	:	std::string
 * 					- string
 *
 * 	e.g.		<document>
 * 					<element>text</element>
 *
 * 	calling parseChildElementText(<document>, "element") returns string "text"
 */
	std::string result;

	Element* targetElement = parentElement.getChildElement(name);
	if ( targetElement )
		result = targetElement->innerText();

	return result;
}


std::string XML_parser::parseElementText(const Element& givenElement)
{
/* 	param  	:	const Element& parentElement
 * 					- element
 *
 * 	return 	:	std::string
 * 					- string
 *
 * 	e.g.		<document>
 * 					<element>text</element>
 *
 * 	calling parseElementText(<element>) returns string "text"
 */
	std::string result;
	result = givenElement.innerText();
	return result;
};


Element* XML_parser::getChildElement(const Element& parentElement, const std::string& name)
{
/* 	param  	:	const Element& parentElement
 * 					- parent element
 * 				const std::string& name
 * 					- name of element
 *
 * 	return 	:	Element*
 * 					- pointer to element
 *
 * 	e.g.		<document>
 * 					<element>text</element>
 *
 * 	calling getChildElement(<document>, "element") returns string pointer to <element>
 */
	Element* targetElement = parentElement.getChildElement(name);
	return targetElement;
}

std::string XML_parser::parseElementAttribute(const Element& givenElement, const std::string& name)
{
/* 	param  	:	const Element& parentElement
 * 					- parent element
 * 				const std::string& name
 * 					- name of element
 *
 * 	return 	:	Element*
 * 					- pointer to element
 *
 * 	e.g.		<document>
 * 					<element>text</element>
 *
 * 	calling getChildElement(<document>, "element") returns string pointer to <element>
 */
	return (givenElement).getAttribute(name);
};



Element* XML_parser::nodeToElement(Node* node)
{
/* 	param  	:	Node* node
 *
 * 	return 	:	Element*
 * 					- pointer to element
 *
 * 	e.g.		<document>
 * 					<element>text</element>
 *
 * 	calling getChildElement(<document>, "element") returns string pointer to <element>
 */
	return dynamic_cast<Element*>(node);
}


unsigned long stringToHexU(const std::string& string)
{
/* 	param  	:	const std::string& string
 * 					- string to be converted
 *
 * 	return 	:	unsigned long
 * 					- pointer to element
 *
 * 	e.g.		<document>
 * 					<element>text</element>
 *
 * 	calling getChildElement(<document>, "element") returns string pointer to <element>
 */
	return 1;
};


long stringToHex(const std::string& string)
{

	return 1;
};

int XML_parser::extractString(const std::string& string, const std::string& start_node, const std::string& end_node, std::string* ptr_result){
	// variables
	//std::string result_string;
	unsigned int xml_string_start;
	unsigned int xml_string_end;

// isolate string from <doc> to </doc> node
	xml_string_start = string.find(start_node);
	xml_string_end = string.find(end_node);

	if( (xml_string_start == std::string::npos) or (xml_string_end == std::string::npos) ){
		return RETURN_ERROR;
	}

	xml_string_end = xml_string_end+end_node.length()-1; // to include the end_node as well

	//result_string = string.substr(xml_string_start, xml_string_end-(xml_string_start-1));

	*ptr_result = string.substr(xml_string_start, xml_string_end-(xml_string_start-1));

	return RETURN_SUCCESS;
}


#endif /* XML_PARSER_HPP_ */
