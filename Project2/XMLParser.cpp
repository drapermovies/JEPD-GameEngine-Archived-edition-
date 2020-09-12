#include <iostream>
#include "XMLParser.h"

XMLParser::XMLParser(SceneManager& new_manager,
					TimerClass& new_timer,
					LightShaderClass& new_shader)
	: scene_manager(new_manager),
	  light_shader(new_shader)
{
	rapidxml::file<> xmlFile("XML/Scenes/test.xml");
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	rapidxml::xml_node<>* pRoot = doc.first_node();

	//GET EVERY NODE LABELLED 'SCENE' AND GENERATE A SCENE

	for (rapidxml::xml_node<>* pNode = pRoot->first_node(); 
										pNode; 
										pNode = pRoot->last_node())
	{
		if (pNode->name() == "Scene")
		{
		}

		if (pNode->first_node()->name == "SceneName")
		{

		}
	}
}