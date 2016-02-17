#ifndef SIGMA_MAKE_LUA
#define SIGMA_MAKE_LUA

#include "Vector.h"
#include "Node.h"
#include "Camera2D.h"
#include "Scene.h"
#include "Input.h"
#include "GameLogic.h"
#include "Component.h"
#include "MessageNetwork.h"

#include <chaiscript/chaiscript.hpp>
using namespace chaiscript;

#include <string>
#include <vector>
#include <initializer_list>

namespace sig {
	using namespace math;

	namespace chai {

		static void RegisterEngineClasses(ChaiScript &chai)
		{
			ModulePtr m = ModulePtr(new Module());
			Module &mod = *m;

			// Register Vector types
			mod.add( user_type<Vector2>(), "Vector2" );
			mod.add( constructor<Vector2()>(), "Vector2" );
			mod.add( constructor<Vector2(const Vector2&)>(), "Vector2" );
			mod.add( constructor<Vector2(float)>(), "Vector2" );
			mod.add( constructor<Vector2(float, float)>(), "Vector2" );
			mod.add( fun(&Vector2::Cross), "cross" );
			mod.add( fun(&Vector2::Dot), "dot" );
			mod.add( fun(&Vector2::Length), "get_length" );
			mod.add( fun(&Vector2::X), "get_x" );
			mod.add( fun(&Vector2::Y), "get_y" );
			mod.add( fun(&Vector2::SetX), "set_x" );
			mod.add( fun(&Vector2::SetY), "set_y" );
			mod.add( fun(&Vector2::Normalized), "normalized" );
			mod.add( fun(&Vector2::operator!=), "!=" );
			mod.add( fun(&Vector2::operator+), "+" );
			mod.add( fun(&Vector2::operator-), "-" );
			mod.add( fun(&Vector2::operator*), "*" );
			mod.add( fun(&Vector2::operator/), "/" );
			mod.add( fun(&Vector2::operator==), "==" );

			// Register Node
			mod.add( user_type<Node>(), "Node" );
			mod.add( constructor<Node()>(), "Node" );
			mod.add( constructor<Node(const Node&)>(), "Node" );
			mod.add( fun(&Node::GetPosition), "get_position" );
			mod.add( fun(&Node::SetPosition), "set_position" );
			mod.add( fun(&Node::GetRotation), "get_rotation" );
			mod.add( fun(&Node::SetRotation), "set_rotation" );
			mod.add( fun(&Node::GetScale), "get_scale" );
			mod.add( fun(&Node::SetScale), "set_scale" );
			mod.add( fun(&Node::GetComponent), "get_component" );
			mod.add( fun(&Node::Instantiate), "instantiate" );
			mod.add( fun(&Node::GetChildInactive), "get_child_inactive" );
			mod.add( fun(&Node::RemoveChildInactive), "remove_child_inactive" );
			mod.add( fun(&Node::GetName), "get_name" );
			mod.add( fun(&Node::GetSize), "get_size" );
			mod.add( fun(&Node::GetParent), "get_parent" );
			mod.add( fun(&Node::SetTag), "set_tag" );
			mod.add( fun(&Node::GetTag), "get_tag" );
			mod.add( fun(&Node::SendMessage), "send_message" );
			mod.add( fun(&Node::GetScene), "get_scene" );

			// Register Collision and Message
			mod.add( user_type<Collision>(), "Collision" );
			mod.add( fun(&Collision::hitNode), "hit_node" );
			mod.add( fun(&Collision::hitNormal), "hit_normal" );
			mod.add( fun(&Collision::hitPosition), "hit_position" );

			mod.add( user_type<Message>(), "Message" );
			mod.add( fun(&Message::GetBody), "get_body");
			mod.add( fun(&Message::GetFrom), "get_from");
			mod.add( fun(&Message::GetTo), "get_to");

			// Register GameLogic component
			mod.add( user_type<GameLogic>(), "GameLogic" );
			mod.add( constructor<GameLogic()>(), "GameLogic" );
			mod.add( constructor<GameLogic(const GameLogic&)>(), "GameLogic" );
			mod.add( fun(&GameLogic::GetOwner), "get_owner");
			mod.add( fun(&GameLogic::IsEnabled), "is_enabled");
			mod.add( fun(&GameLogic::SetEnabled), "set_enabled");
			mod.add( fun(&GameLogic::OnCollisionEnter), "OnCollisionEnter");
			mod.add( fun(&GameLogic::OnCollisionExit), "OnCollisionExit");
			mod.add( fun(&GameLogic::OnInitialize), "OnInitialize");
			mod.add( fun(&GameLogic::OnMessageReceived), "OnMessageReceived");
			mod.add( fun(&GameLogic::OnUpdate), "OnUpdate");

			// Register Camera2D
			mod.add( user_type<Camera2D>(), "Camera2D");
			mod.add( base_class<Node, Camera2D>() );
			mod.add( constructor<Camera2D()>(), "Camera2D" );
			mod.add( constructor<Camera2D(const Camera2D&)>(), "Camera2D" );
			mod.add( fun(&Camera2D::GetZoom), "get_zoom" );
			mod.add( fun(&Camera2D::SetZoom), "set_zoom" );

			// Register Scene
			mod.add( user_type<Scene>(), "Scene");
			mod.add( constructor<Scene(const Scene&)>(), "Scene" );
			mod.add( fun(static_cast<std::vector<Node*> (Scene::*)()>(&Scene::GetAllNodes)), "get_all_nodes" );
			mod.add( fun(&Scene::GetBackground), "get_back_color" );
			mod.add( fun(&Scene::SetBackground), "set_back_color" );
			mod.add( fun(&Scene::GetCamera), "get_active_camera" );
			mod.add( fun(&Scene::SetCamera), "set_active_camera" );
			mod.add( fun(&Scene::Instantiate), "instantiate" );
			mod.add( fun(&Scene::AddChildInactive), "add_child_inactive" );
			mod.add( fun(&Scene::GetChildInactive), "get_child_inactive" );
			mod.add( fun(&Scene::RemoveChildInactive), "remove_child_inactive" );
			mod.add( fun(&Scene::SendMessage), "send_message" );

			// Register Input
			ModulePtr inm = ModulePtr(new Module());
			Module &im = *inm;

			im.add( fun(&Input::GetKey), "get_key" );
			im.add( fun(&Input::GetKeyDown), "get_key_down" );
			im.add( fun(&Input::GetKeyUp), "get_key_up" );
			im.add( fun(&Input::GetModifier), "get_modifier" );
			im.add( fun(&Input::GetMouse), "get_mouse" );
			im.add( fun(&Input::GetMouseButtonDown), "get_mouse_button_down" );
			im.add( fun(&Input::GetMouseButtonUp), "get_mouse_button_up" );
			im.add( fun(&Input::GetMousePosition), "get_mouse_position" );

			im.add_global_const( const_var(Input::KEY_0), "KEY_0" );
			im.add_global_const( const_var(Input::KEY_1), "KEY_1" );
			im.add_global_const( const_var(Input::KEY_2), "KEY_2" );
			im.add_global_const( const_var(Input::KEY_3), "KEY_3" );
			im.add_global_const( const_var(Input::KEY_4), "KEY_4" );
			im.add_global_const( const_var(Input::KEY_5), "KEY_5" );
			im.add_global_const( const_var(Input::KEY_6), "KEY_6" );
			im.add_global_const( const_var(Input::KEY_7), "KEY_7" );
			im.add_global_const( const_var(Input::KEY_8), "KEY_8" );
			im.add_global_const( const_var(Input::KEY_9), "KEY_9" );

			im.add_global_const( const_var(Input::KEY_KP_0), "KEY_KP_0" );
			im.add_global_const( const_var(Input::KEY_KP_1), "KEY_KP_1" );
			im.add_global_const( const_var(Input::KEY_KP_2), "KEY_KP_2" );
			im.add_global_const( const_var(Input::KEY_KP_3), "KEY_KP_3" );
			im.add_global_const( const_var(Input::KEY_KP_4), "KEY_KP_4" );
			im.add_global_const( const_var(Input::KEY_KP_5), "KEY_KP_5" );
			im.add_global_const( const_var(Input::KEY_KP_6), "KEY_KP_6" );
			im.add_global_const( const_var(Input::KEY_KP_7), "KEY_KP_7" );
			im.add_global_const( const_var(Input::KEY_KP_8), "KEY_KP_8" );
			im.add_global_const( const_var(Input::KEY_KP_9), "KEY_KP_9" );

			im.add_global_const( const_var(Input::KEY_F1), "KEY_F1" );
			im.add_global_const( const_var(Input::KEY_F2), "KEY_F2" );
			im.add_global_const( const_var(Input::KEY_F3), "KEY_F3" );
			im.add_global_const( const_var(Input::KEY_F4), "KEY_F4" );
			im.add_global_const( const_var(Input::KEY_F5), "KEY_F5" );
			im.add_global_const( const_var(Input::KEY_F6), "KEY_F6" );
			im.add_global_const( const_var(Input::KEY_F7), "KEY_F7" );
			im.add_global_const( const_var(Input::KEY_F8), "KEY_F8" );
			im.add_global_const( const_var(Input::KEY_F9), "KEY_F9" );
			im.add_global_const( const_var(Input::KEY_F10), "KEY_F10" );
			im.add_global_const( const_var(Input::KEY_F11), "KEY_F11" );
			im.add_global_const( const_var(Input::KEY_F12), "KEY_F12" );

			im.add_global_const( const_var(Input::KEY_A), "KEY_A" );
			im.add_global_const( const_var(Input::KEY_B), "KEY_B" );
			im.add_global_const( const_var(Input::KEY_C), "KEY_C" );
			im.add_global_const( const_var(Input::KEY_D), "KEY_D" );
			im.add_global_const( const_var(Input::KEY_E), "KEY_E" );
			im.add_global_const( const_var(Input::KEY_F), "KEY_F" );
			im.add_global_const( const_var(Input::KEY_G), "KEY_G" );
			im.add_global_const( const_var(Input::KEY_H), "KEY_H" );
			im.add_global_const( const_var(Input::KEY_I), "KEY_I" );
			im.add_global_const( const_var(Input::KEY_J), "KEY_J" );
			im.add_global_const( const_var(Input::KEY_K), "KEY_K" );
			im.add_global_const( const_var(Input::KEY_L), "KEY_L" );
			im.add_global_const( const_var(Input::KEY_M), "KEY_M" );
			im.add_global_const( const_var(Input::KEY_N), "KEY_N" );
			im.add_global_const( const_var(Input::KEY_O), "KEY_O" );
			im.add_global_const( const_var(Input::KEY_P), "KEY_P" );
			im.add_global_const( const_var(Input::KEY_Q), "KEY_Q" );
			im.add_global_const( const_var(Input::KEY_R), "KEY_R" );
			im.add_global_const( const_var(Input::KEY_S), "KEY_S" );
			im.add_global_const( const_var(Input::KEY_T), "KEY_T" );
			im.add_global_const( const_var(Input::KEY_U), "KEY_U" );
			im.add_global_const( const_var(Input::KEY_V), "KEY_V" );
			im.add_global_const( const_var(Input::KEY_W), "KEY_W" );
			im.add_global_const( const_var(Input::KEY_X), "KEY_X" );
			im.add_global_const( const_var(Input::KEY_Y), "KEY_Y" );
			im.add_global_const( const_var(Input::KEY_Z), "KEY_Z" );

			im.add_global_const( const_var(Input::KEY_BACKSLASH), "KEY_BACKSLASH" );
			im.add_global_const( const_var(Input::KEY_SLASH), "KEY_SLASH" );
			im.add_global_const( const_var(Input::KEY_SPACE), "KEY_SPACE" );
			im.add_global_const( const_var(Input::KEY_BACKSPACE), "KEY_BACKSPACE" );
			im.add_global_const( const_var(Input::KEY_LEFTBRACKET), "KEY_LEFTBRACKET" );
			im.add_global_const( const_var(Input::KEY_RIGHTBRACKET), "KEY_RIGHTBRACKET" );
			im.add_global_const( const_var(Input::KEY_TAB), "KEY_TAB" );
			im.add_global_const( const_var(Input::KEY_CAPSLOCK), "KEY_CAPSLOCK" );
			im.add_global_const( const_var(Input::MOD_SHIFT), "MOD_SHIFT" );
			im.add_global_const( const_var(Input::MOD_CTRL), "MOD_CTRL" );
			im.add_global_const( const_var(Input::KEY_PERIOD), "KEY_PERIOD" );
			im.add_global_const( const_var(Input::KEY_COMMA), "KEY_COMMA" );
			im.add_global_const( const_var(Input::KEY_KP_COMMA), "KEY_KP_COMMA" );
			im.add_global_const( const_var(Input::KEY_KP_DIVIDE), "KEY_KP_DIVIDE" );
			im.add_global_const( const_var(Input::KEY_KP_MULTIPLY), "KEY_KP_MULTIPLY" );
			im.add_global_const( const_var(Input::KEY_KP_MINUS), "KEY_KP_MINUS" );
			im.add_global_const( const_var(Input::KEY_KP_PLUS), "KEY_KP_PLUS" );
			im.add_global_const( const_var(Input::KEY_KP_PERIOD), "KEY_KP_PERIOD" );
			im.add_global_const( const_var(Input::KEY_KP_ENTER), "KEY_KP_ENTER" );
			im.add_global_const( const_var(Input::KEY_NUMLOCKCLEAR), "KEY_NUMLOCK" );
			im.add_global_const( const_var(Input::KEY_INSERT), "KEY_INSERT" );
			im.add_global_const( const_var(Input::KEY_DELETE), "KEY_DELETE" );
			im.add_global_const( const_var(Input::KEY_HOME), "KEY_HOME" );
			im.add_global_const( const_var(Input::KEY_END), "KEY_END" );
			im.add_global_const( const_var(Input::KEY_PAGEUP), "KEY_PAGEUP" );
			im.add_global_const( const_var(Input::KEY_PAGEDOWN), "KEY_PAGEDOWN" );
			im.add_global_const( const_var(Input::KEY_PRINTSCREEN), "KEY_PRINTSCREEN" );
			im.add_global_const( const_var(Input::KEY_SCROLLLOCK), "KEY_SCROLLLOCK" );
			im.add_global_const( const_var(Input::KEY_PAUSE), "KEY_PAUSE" );

			im.add_global_const( const_var(Input::KEY_UP), "KEY_UP" );
			im.add_global_const( const_var(Input::KEY_DOWN), "KEY_DOWN" );
			im.add_global_const( const_var(Input::KEY_LEFT), "KEY_LEFT" );
			im.add_global_const( const_var(Input::KEY_RIGHT), "KEY_RIGHT" );

			im.add( user_type<Input>(), "Input" );

			chai.add(m);
			chai.add(inm);
		}
	}
}

#endif // SIGMA_MAKE_LUA
