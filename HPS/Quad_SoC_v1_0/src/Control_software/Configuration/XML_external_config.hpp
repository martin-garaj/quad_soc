//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Diagnostic system of photo-voltaic panels           //
//                                                                //
//================================================================//
#ifndef _XML_EXTERNAL_CONFIG_HPP_
#define _XML_EXTERNAL_CONFIG_HPP_

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

#include <Poco/DOM/Comment.h>
using Poco::XML::Comment;

#include "XML_parser.hpp"
#include "system_const.h"
#include "API_Configuration.hpp"


#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <iostream>
#include <fstream>
#include <string>     // std::string, std::stod

//================================================================================================//
//                                                                								  //
//	CLASS	: XML_input_info class is responsible for parsing xml file with input information 	  //
//			  (input_info.xml) and store it in Input object (input_object.hpp)     				  //
//                                                              								  //
//================================================================================================//
class XML_external_config: public XML_parser {

public:
	XML_external_config(){};
	~XML_external_config(){};
	int parse(const std::string xml_external_config, Configuration_object* p_config_object);
	int write(const std::string xml_external_config, Configuration_object* p_config_object);

private:
	//TODO int parse_config();
	//TODO int parse_config_radio(Node* node);
	//TODO int parse_config_radio_channel(Node* node, int which_channel, Channel_struct * return_struct);
	//TODO int parse_config_sensor(Node* node);
	//TODO int parse_config_sensor_mpu(Node* node, int which_mpu, MPU_struct * return_struct);

/*
	int parse_system_info(Node* node, int* system_server_id, int* system_zigbee_id);
	int parse_input_time(Node* node, time_structure* input_time);
	int parse_wifi_info(Node* node, wifi_info_structure* wifi_info);
	int parse_server_info(Node* node, server_info_structure* server_info);
	int parse_event_list(NodeList* nodes, int node_count, event_list_structure* event_list);
	int parse_panel_list(NodeList* nodes, int node_count, panel_list_structure* panel_list);
	*/

};


int XML_external_config::parse(const std::string xml_external_config, Configuration_object* p_config_object){
	//==============================================================================================//
	//												VARIABLES											//
	//==============================================================================================//
	std::ifstream xml_file;
	int count_Element_control;
	int count_Element_control_pid;
	int count_Element_radio;
	int count_Element_radio_protocol;
	int count_Element_radio_channel;
	int count_Element_sensor;
	int count_Element_sensor_imu;

	// function
	try{
		//==============================================================================================//
		//											XML FILE											//
		//==============================================================================================//
		// open the file
		char *file_name = (char*)xml_external_config.c_str();
		xml_file.open(file_name, std::fstream::in); // reading
		InputSource src(xml_file);

		// initialize DOM parser
		DOMParser parser;
		AutoPtr<Document> doc = parser.parse(&src);
		// get the root element <config> (omitted from further tree reference, e.g. <config> => <radio> => <channel> is <radio> => <channel>)
		Element* Element_config = doc->documentElement();
		//==============================================================================================//
		//												PARSE											//
		//==============================================================================================//
		//------------------------------------------------------------------------------------//
		//--------------------------------PARSE <control>-------------------------------------//
		//------------------------------------------------------------------------------------//
		// get the number of <control>
		count_Element_control = this->getNodesAmount(*Element_config, "control");

		if(count_Element_control != 1){
			std::cout << "ERROR XML_external_config::parse -> Element_control expected 1, found " << count_Element_control << std::endl;
			return RETURN_ERROR;
		}else{
			// locate <control>
			NodeList * NodeList_control;
			NodeList_control = this->getNodeList(*Element_config, "control");
			// convert NodeList_control->item(0) to Element
			Element * Element_control = dynamic_cast<Element*>(NodeList_control->item(0));

			//------------------------------------------------------------------------------------//
			//--------------------------------PARSE <control> => <pid>----------------------------//
			//------------------------------------------------------------------------------------//
			// get the number of <control> => <pid>
			count_Element_control_pid = this->getNodesAmount(*Element_control, "pid");
			if(count_Element_control_pid > CONFIG_CONTROL_PID_COUNT){
				std::cout << "ERROR XML_external_config::parse -> count_Element_control_pid = " << count_Element_control_pid << std::endl;
				return RETURN_ERROR;
			}


			// locate <control> => <pid>
			NodeList * NodeList_pid;
			NodeList_pid = this->getNodeList(*Element_control, "pid");

			// parse <control> => <pid>
			Element * Element_pid;
			for(int i = 0; i < CONFIG_CONTROL_PID_COUNT; i++){
				Element_pid = dynamic_cast<Element*>(NodeList_pid->item(i));

				try{
					// extract the values from <channel>
					double id = std::stod( (*Element_pid).getAttribute("id"));
					double T_d = std::stod( parseChildElementText((*Element_pid), "T_d"));
					double P = std::stod( parseChildElementText((*Element_pid), "P"));
					double I = std::stod( parseChildElementText((*Element_pid), "I"));
					double D = std::stod( parseChildElementText((*Element_pid), "D"));

					// set the Configuration object
					p_config_object->set_pid(id, T_d, P, I, D);
				}
				catch (Exception& exc)
				{
					std::cerr << exc.displayText() << std::endl;
					return RETURN_ERROR;
				}
			}

		}

		//------------------------------------------------------------------------------------//
		//--------------------------------PARSE <radio>---------------------------------------//
		//------------------------------------------------------------------------------------//
		// get the number of <radio>
		count_Element_radio = this->getNodesAmount(*Element_config, "radio");

		if(count_Element_radio == 1){
			// locate <radio>
			NodeList * NodeList_radio;
			NodeList_radio = this->getNodeList(*Element_config, "radio");
			// convert NodeList_radio->item(0) to Element
			Element * Element_radio = dynamic_cast<Element*>(NodeList_radio->item(0));

			//------------------------------------------------------------------------------------//
			//--------------------------------PARSE <radio> => <protocol>-------------------------//
			//------------------------------------------------------------------------------------//
			// get the number of <radio> => <protocol>
			count_Element_radio_protocol = this->getNodesAmount(*Element_radio, "protocol");
			if(count_Element_radio_protocol != 1){
				std::cout << "ERROR XML_external_config::parse -> count_Element_radio_protocol = " << count_Element_radio_protocol << ", exactly "<< 1 <<" has to be defined"<< std::endl;
				return RETURN_ERROR;
			}

			// parse <radio> => <protocol>
			int protocol = std::stoi( parseChildElementText((*Element_radio), "protocol"), nullptr, 10);

			// set the Configuration object
			p_config_object->set_protocol(protocol);

			//------------------------------------------------------------------------------------//
			//--------------------------------PARSE <radio> => <channel>--------------------------//
			//------------------------------------------------------------------------------------//
			// get the number of <radio> => <channel>
			count_Element_radio_channel = this->getNodesAmount(*Element_radio, "channel");
			if(count_Element_radio_channel != CONFIG_RADIO_CHANNEL_COUNT){
				std::cout << "ERROR XML_external_config::parse -> count_Element_radio_channel = " << count_Element_radio_channel << ", exactly "<< CONFIG_RADIO_CHANNEL_COUNT <<" have to be defined"<< std::endl;
				return RETURN_ERROR;
			}


			// locate <radio> => <channel>
			NodeList * NodeList_channel;
			NodeList_channel = this->getNodeList(*Element_radio, "channel");

			// parse <radio> => <channel>
			Element * Element_channel;
			for(int i = 0; i < CONFIG_RADIO_CHANNEL_COUNT; i++){
				Element_channel = dynamic_cast<Element*>(NodeList_channel->item(i));

				try{
					// extract the values from <channel>
					int id = std::stoi( (*Element_channel).getAttribute("id"), nullptr, 10);
					int limit_min = std::stoi( parseChildElementText((*Element_channel), "limit_min"), nullptr, 10);
					int center_value = std::stoi( parseChildElementText((*Element_channel), "center_value"), nullptr, 10);
					int limit_max = std::stoi( parseChildElementText((*Element_channel), "limit_max"), nullptr, 10);

					// set the Configuration object
					p_config_object->set_channel(id, limit_min, center_value, limit_max);
				}
				catch (Exception& exc)
				{
					std::cout << "ERROR XML_external_config::parse -> Element_channel(" << i << ")" << std::endl;
					std::cerr << exc.displayText() << std::endl;
					return RETURN_ERROR;
				}
			}
		}else{
			std::cout << "ERROR XML_external_config::parse -> Element_radio expected 1, found " << count_Element_radio << std::endl;
			return RETURN_ERROR;
		}

		//------------------------------------------------------------------------------------//
		//--------------------------------PARSE <sensor>--------------------------------------//
		//------------------------------------------------------------------------------------//
		// get the number of <sensor>
		count_Element_sensor = this->getNodesAmount(*Element_config, "sensor");

		if(count_Element_sensor == 1){
			// locate <sensor>
			NodeList * NodeList_sensor;
			NodeList_sensor = this->getNodeList(*Element_config, "sensor");
			// convert NodeList_sensor->item(0) to Element
			Element * Element_sensor = dynamic_cast<Element*>(NodeList_sensor->item(0));


			//------------------------------------------------------------------------------------//
			//--------------------------------PARSE <sensor> => <imu>-----------------------------//
			//------------------------------------------------------------------------------------//
			// get the number of <radio> => <channel>
			count_Element_sensor_imu = this->getNodesAmount(*Element_sensor, "imu");
			if(count_Element_sensor_imu > CONFIG_SENSOR_IMU_COUNT){
				count_Element_sensor_imu = CONFIG_SENSOR_IMU_COUNT;
				std::cout << "WARNING XML_external_config::parse -> count_Element_sensor_imu = " << count_Element_sensor_imu << ", corrected to "<< CONFIG_SENSOR_IMU_COUNT << std::endl;
			}else if(count_Element_sensor_imu < 1){
				std::cout << "ERROR XML_external_config::parse -> count_Element_sensor_imu = " << count_Element_sensor_imu << ", expected at least 1" << std::endl;
				return RETURN_ERROR;
			}


			// locate <sensor> => <imu>
			NodeList * NodeList_imu;
			NodeList_imu = this->getNodeList(*Element_sensor, "imu");

			// parse <sensor> => <channel>
			Element * Element_imu;
			for(int i = 0; i < CONFIG_SENSOR_IMU_COUNT; i++){
				Element_imu = dynamic_cast<Element*>(NodeList_imu->item(i));

				try{
					// extract the values from <channel>
					int id = std::stoi( (*Element_imu).getAttribute("id"), nullptr, 10);
					int component = std::stoi( parseChildElementText((*Element_imu), "component"), nullptr, 10);
					int version = std::stoi( parseChildElementText((*Element_imu), "version"), nullptr, 10);
					int state = std::stoi( parseChildElementText((*Element_imu), "state"), nullptr, 10);
					uint8_t i2c_address = std::stoi( parseChildElementText((*Element_imu), "i2c_address"), nullptr, 16);

					// set the Configuration object
					p_config_object->set_imu(id, component, version, state, i2c_address);

				}
				catch (Exception& exc)
				{
					std::cout << "ERROR XML_external_config::parse -> Element_imu(" << i << ")" << std::endl;
					std::cerr << exc.displayText() << std::endl;
					return RETURN_ERROR;
				}
			}
		}else{
			std::cout << "ERROR XML_external_config::parse -> Element_sensor expected 1, found " << count_Element_sensor << std::endl;
			return RETURN_ERROR;
		}

		//------------------------------------------------------------------------------------//
		//--------------------------------CLOSE XML FILE--------------------------------------//
		//------------------------------------------------------------------------------------//
		xml_file.close();


	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
};




int XML_external_config::write(const std::string xml_external_config, Configuration_object* p_config_object){
	//==============================================================================================//
	//												VARIABLES											//
	//==============================================================================================//
	std::ofstream xml_file;
	// function
	try{
		//==============================================================================================//
		//											XML FILE											//
		//==============================================================================================//
		// open the file
		char *file_name = (char*)xml_external_config.c_str();
		xml_file.open(file_name, std::fstream::out); // writing
		if(!xml_file.is_open()){
			std::cout << "ERROR XML_external_config::write -> xml file didnt open"<< std::endl;
			return RETURN_ERROR;
		}
		// create xml document
		AutoPtr<Document> xml_doc = new Document;
		// root element
		AutoPtr<Element> Element_config = xml_doc->createElement("config");
		// <config>
		xml_doc->appendChild(Element_config);
		// <config> comment
		AutoPtr<Comment> Element_config_comment = xml_doc->createComment(	"This is configuration file to store user-adjustable parameters \n"
																			"these parameters are assumed to be adjusted whenever there is any \n"
																			"hardware change.");
		Element_config->appendChild(Element_config_comment);
		//==============================================================================================//
		//												WRITE											//
		//==============================================================================================//
		//------------------------------------------------------------------------------------//
		//--------------------------------WRITE <control>-------------------------------------//
		//------------------------------------------------------------------------------------//

		// <config> => <control>
		AutoPtr<Element> Element_control = xml_doc->createElement("control");
		Element_config->appendChild(Element_control);

		// <control> comment
		AutoPtr<Comment> Element_control_comment = xml_doc->createComment(	"<control> stores parameters regarding control algorithms\n"
																			"<pid id=0> YAW\n"
																			"<pid id=1> PITCH\n"
																			"<pid id=2> ROLL\n");
		Element_control->appendChild(Element_control_comment);

		//------------------------------------------------------------------------------------//
		//--------------------------------WRITE <control> => <pid>--------------------------//
		//------------------------------------------------------------------------------------//
		PID_struct temp_pid;
		for(int i = 0; i<CONFIG_CONTROL_PID_COUNT; i++){
			if(p_config_object->get_pid(i, &temp_pid) == RETURN_SUCCESS){
				// create <channel>
				AutoPtr<Element> Element_pid = xml_doc->createElement("pid");
				// set <pid "id"="value">
				Element_pid->setAttribute("id", std::to_string( i ) );
				// create <T_d>, <P>, <I>, <D>
				AutoPtr<Element> Element_T_d = xml_doc->createElement("T_d");
				AutoPtr<Element> Element_P = xml_doc->createElement("P");
				AutoPtr<Element> Element_I = xml_doc->createElement("I");
				AutoPtr<Element> Element_D = xml_doc->createElement("D");
				// create "value"
				AutoPtr<Text> Node_T_d = xml_doc->createTextNode( std::to_string( temp_pid.T_d ) );
				AutoPtr<Text> Node_P = xml_doc->createTextNode( std::to_string( temp_pid.P ) );
				AutoPtr<Text> Node_I = xml_doc->createTextNode( std::to_string( temp_pid.I ) );
				AutoPtr<Text> Node_D = xml_doc->createTextNode( std::to_string( temp_pid.D ) );
				// set <T_d> "value", <P> "value", <I> "value", <D> "value"
				Element_T_d->appendChild(Node_T_d);
				Element_P->appendChild(Node_P);
				Element_I->appendChild(Node_I);
				Element_D->appendChild(Node_D);
				// <channel> => <limit_min>
				Element_pid->appendChild(Element_T_d);
				Element_pid->appendChild(Element_P);
				Element_pid->appendChild(Element_I);
				Element_pid->appendChild(Element_D);
				// <radio> => <channel "id"="value"> => <limit_min>/<center_value>/<limit_max>
				Element_control->appendChild(Element_pid);

				// comment
				if(i == 0){
					AutoPtr<Comment> Element_pid_comment = xml_doc->createComment("PID_roll - X AXIS");
					Element_pid->appendChild(Element_pid_comment);
				}
				if(i == 1){
					AutoPtr<Comment> Element_pid_comment = xml_doc->createComment("PID_pitch - Y AXIS");
					Element_pid->appendChild(Element_pid_comment);
				}
				if(i == 2){
					AutoPtr<Comment> Element_pid_comment = xml_doc->createComment("PID_yaw - Z AXIS");
					Element_pid->appendChild(Element_pid_comment);
				}

			}else{
				std::cout << "ERROR XML_external_config::write <control> => <pid> not defined"<< std::endl;
			};

		}

		//------------------------------------------------------------------------------------//
		//--------------------------------WRITE <radio>---------------------------------------//
		//------------------------------------------------------------------------------------//

		// <config> => <radio>
		AutoPtr<Element> Element_radio = xml_doc->createElement("radio");
		Element_config->appendChild(Element_radio);
		// <control> comment
		AutoPtr<Comment> Element_radio_comment = xml_doc->createComment(	"<radio> stores values regarding radio, so tuning needs to be done only once for every new radio\n"
																			"<channel id=0> right stick vertical\n"
																			"<channel id=1> right stick horizontal\n"
																			"<channel id=2> left stick vertical\n"
																			"<channel id=3> left stick horizontal\n"
																			"<channel id=4> user-adjustable\n");
		Element_radio->appendChild(Element_radio_comment);

			//------------------------------------------------------------------------------------//
			//--------------------------------WRITE <radio> => <protocol>-------------------------//
			//------------------------------------------------------------------------------------//
			int protocol;
			if(p_config_object->get_protocol(&protocol) == RETURN_SUCCESS){
				// create <protocol>
				AutoPtr<Element> Element_protocol = xml_doc->createElement("protocol");
				// create "value"
				AutoPtr<Text> Node_protocol = xml_doc->createTextNode( std::to_string( protocol ) );
				// <protocol> "value" </protocol>
				Element_protocol->appendChild(Node_protocol);
				// <radio> => <protocol>
				Element_radio->appendChild(Element_protocol);
			}else{
				std::cout << "ERROR XML_external_config::write <radio> => <protocol> not defined"<< std::endl;
			};

			//------------------------------------------------------------------------------------//
			//--------------------------------WRITE <radio> => <channel>--------------------------//
			//------------------------------------------------------------------------------------//
			Channel_struct temp_channel;
			for(int i = 0; i<CONFIG_RADIO_CHANNEL_COUNT; i++){
				if(p_config_object->get_channel(i, &temp_channel) == RETURN_SUCCESS){
					// create <channel>
					AutoPtr<Element> Element_channel = xml_doc->createElement("channel");
					// set <channel "id"="value">
					Element_channel->setAttribute("id", std::to_string( i ) );
					// create <limit_min>, <center_value>, <limit_max>
					AutoPtr<Element> Element_limit_min = xml_doc->createElement("limit_min");
					AutoPtr<Element> Element_center_value = xml_doc->createElement("center_value");
					AutoPtr<Element> Element_limit_max = xml_doc->createElement("limit_max");
					// create "value"
					AutoPtr<Text> Node_limit_min = xml_doc->createTextNode( std::to_string( temp_channel.limit_min ) );
					AutoPtr<Text> Node_center_value = xml_doc->createTextNode( std::to_string( temp_channel.center_value ) );
					AutoPtr<Text> Node_limit_max = xml_doc->createTextNode( std::to_string( temp_channel.limit_max ) );
					// set <limit_min> "value", <center_value> "value", <limit_max> "value"
					Element_limit_min->appendChild(Node_limit_min);
					Element_center_value->appendChild(Node_center_value);
					Element_limit_max->appendChild(Node_limit_max);
					// <channel> => <limit_min>
					Element_channel->appendChild(Element_limit_min);
					Element_channel->appendChild(Element_center_value);
					Element_channel->appendChild(Element_limit_max);
					// <radio> => <channel "id"="value"> => <limit_min>/<center_value>/<limit_max>
					Element_radio->appendChild(Element_channel);
				}else{
					std::cout << "ERROR XML_external_config::write <radio> => <channel> not defined"<< std::endl;
				};

			}

		//------------------------------------------------------------------------------------//
		//--------------------------------WRITE <sensor>--------------------------------------//
		//------------------------------------------------------------------------------------//
		// <config> => <sensor>
		AutoPtr<Element> Element_sensor = xml_doc->createElement("sensor");
		Element_config->appendChild(Element_sensor);
		// <control> comment
		AutoPtr<Comment> Element_sensor_comment = xml_doc->createComment(	"<sensor> stores parameters of sensors\n"
																			);
		Element_sensor->appendChild(Element_sensor_comment);
			//------------------------------------------------------------------------------------//
			//--------------------------------WRITE <sensor> => <imu>-----------------------------//
			//------------------------------------------------------------------------------------//
			IMU_struct temp_imu;
			for(int i = 0; i<CONFIG_SENSOR_IMU_COUNT; i++){
				if(p_config_object->get_imu(i, &temp_imu) == RETURN_SUCCESS){
					// create <imu>
					AutoPtr<Element> Element_imu = xml_doc->createElement("imu");
					// set <imu "id"="value">
					Element_imu->setAttribute("id", std::to_string( i ) );
					// create <component>, <version>, <state>, <i2c_address>
					AutoPtr<Element> Element_component = xml_doc->createElement("component");
					AutoPtr<Element> Element_version = xml_doc->createElement("version");
					AutoPtr<Element> Element_state = xml_doc->createElement("state");
					AutoPtr<Element> Element_i2c_address = xml_doc->createElement("i2c_address");
					// create "value"
					AutoPtr<Text> Node_component = xml_doc->createTextNode( std::to_string( temp_imu.component ) );
					AutoPtr<Text> Node_version = xml_doc->createTextNode( std::to_string( temp_imu.version ) );
					AutoPtr<Text> Node_state = xml_doc->createTextNode( std::to_string( temp_imu.state ) );

					// int to HEX conversion
					std::stringstream ss;	// stringstream used to format the data (HEX)
					std::string string_0x ("0x");
					ss << std::hex << (int)temp_imu.i2c_address;
					std::string string_hexadecimal_value = ss.str(); // string
					string_hexadecimal_value.insert (0, string_0x); // pre-append "0x"
					ss.str(std::string()); // reset stringstream

					// create "value"
					AutoPtr<Text> Node_i2c_address = xml_doc->createTextNode( string_hexadecimal_value );
					// set <limit_min> "value", <center_value> "value", <limit_max> "value"
					Element_component->appendChild(Node_component);
					Element_version->appendChild(Node_version);
					Element_state->appendChild(Node_state);
					Element_i2c_address->appendChild(Node_i2c_address);
					// <channel> => <limit_min>
					Element_imu->appendChild(Element_component);
					Element_imu->appendChild(Element_version);
					Element_imu->appendChild(Element_state);
					Element_imu->appendChild(Element_i2c_address);
					// <radio> => <channel "id"="value"> => <limit_min>/<center_value>/<limit_max>
					Element_sensor->appendChild(Element_imu);
				}else{
					std::cout << "ERROR XML_external_config::write <radio> => <imu> not defined"<< std::endl;
				};

			}

		//------------------------------------------------------------------------------------//
		//--------------------------------WRITE XML FILE--------------------------------------//
		//------------------------------------------------------------------------------------//
		xml_doc->normalize();

		DOMWriter writer;
		writer.setNewLine("\n");
		writer.setOptions(XMLWriter::PRETTY_PRINT);
		writer.writeNode(xml_file, xml_doc);
		//------------------------------------------------------------------------------------//
		//--------------------------------CLOSE XML FILE--------------------------------------//
		//------------------------------------------------------------------------------------//
		xml_file.close();
	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;

}






#endif // _XML_EXTERNAL_CONFIG_HPP_
