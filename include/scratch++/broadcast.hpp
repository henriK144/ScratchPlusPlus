#pragma once 

#include <deque>

using std::deque;
namespace SPP {
enum Message 
{ // An information flag sent as part of a broadcast. 
  // It states, to some amount of specificity, what the broadcast means.
	QUERY_TIME,

	PLAY_SOUND,
	PLAY_MUSIC,
	SILENCE_SOUND,
	SILENCE_MUSIC,

	GAME_STARTED,
	GAME_PAUSED,
	GAME_UNPAUSED,
	GAME_ENDED,

	SHOW_ENTITY,
	HIDE_ENTITY,
	KILL_ENTITY,

	HIDE_ENTITIES_TAGGED,
	KILL_ENTITIES_TAGGED,
	
	CHANGE_VISIBILITY_ENTITY,

	// Define your own broadcasts here

	DUMMYGAME_SCORE_UP,
	DUMMYGAME_SCORE_DOWN,
	DUMMYGAME_SCORE_SOUND,
	DUMMYGAME_COIN_GET,

	// etc...
};

struct Broadcast
{ // A signal that conveys some command and/or information to a game and its systems.
	Message message;
	void* data_ptr = nullptr;
	int data_int = 0;
}; // data_ptr's object may contain additional details for the operation; its type must be inferred from message.
   // Similarly, data_int provides a way for an integer to be specified.
   // Both can be ignored if they are not needed.

class BroadcastQueue
{ // A static class that collects and dispatches broadcasts to the game to update its state.
private:
	static deque<Broadcast> received; // A list of broadcasts receieved
	BroadcastQueue() {} // This class is static, so an instance should not be constructed
public:
	static void receive(Broadcast b) { received.push_back(b); } // Adds an entry to the back of the queue
	static Broadcast extract() // Removes the entry at the front of the queue and returns it
	{
		Broadcast extracted = received.front(); 
		received.pop_front();
		return extracted; 
	} 
	static bool emptyHuh() { return received.empty(); }
};
inline deque<Broadcast> BroadcastQueue::received;

inline void send(Broadcast b) { BroadcastQueue::receive(b); } // Sends a broadcast to the queue
}
