#ifndef GDPUREDATA_H
#define GDPUREDATA_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <PdBase.hpp>

namespace godot {

class GDPureDataHandle : public AudioStreamPlayer {
	GDCLASS(GDPureDataHandle, AudioStreamPlayer)

private:
	pd::PdBase instance;
	float inbuf[1];
	float outbuf[44100 * 2]; //2 channels
	Ref<AudioStreamGenerator> audiostream;

protected:
	static void _bind_methods();

public:
	GDPureDataHandle();
	~GDPureDataHandle();

	double sample_rate = 44100;

	void _ready() override;
	void _process(double delta) override;
	
	bool initialize(int, int, int, bool);
	void clear();

	void add_to_pd_search_paths(String);
	void clear_search_path();

	bool open_patch(String);
	void close_patch(String);
	//void close_patch(pd::Patch);


	//process methods, not exposed
	bool process_float(int, float[], float[]);
	bool process_short(int, short[], short[]);
	bool process_double(int, double[], double[]);
	bool process_raw(float[], float[]);
	bool process_raw_short(short[], short[]);
	bool process_raw_double(double[], double[]);
	//

	void compute_audio(bool);

	//recv functions
	void unsubscribe(String);
	void subscribe(String);
	bool source_exists(String);
	void unsubscribe_all();
	//

	//send functions
	void send_bang(String);
	void send_float(String, float);
	void send_symbol(String, String);
	//
	
	//sending compound msgs
	void start_message();
	void add_float(float);
	void add_symbol(String);
	void finish_list(String);
	void finish_message(String, String);
	//

	//sending midi
	void midi_send_note_on(int, int, int);
	void midi_send_control_change(int, int, int);
	void midi_send_program_change(int, int);
	void midi_send_pitch_bend(int, int);
	void midi_send_aftertouch(int, int);
	void midi_send_poly_aftertouch(int, int, int);
	//

	//sending raw_midi_bytes
	void midi_send_byte(int, int);
	void send_systex(int, int);
	void send_sys_real_time(int, int);
	//

	//pd array methods, not exposed
	bool read_array(String, Array, int, int);
	bool write_array(String, Array, int, int);
	void clear_array(String, int);
	//

	//util methods
	bool is_initialized(){
		return instance.isInited();
	}
	bool is_queued(){
		return instance.isQueued();
	}
	int block_size(){
		return pd::PdBase::blockSize();
	}
	void set_max_message_length(int length){
		if (length < 0){
			std::cerr << "Length cannot be less than 0!";
			return;
		}
		instance.setMaxMessageLen(length);
	}
	int max_message_length(){
		return instance.maxMessageLen();
	}

};

}

#endif