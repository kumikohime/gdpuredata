#include <gdpuredatahandle.h>
#include <godot_cpp/classes/audio_stream_generator.hpp>

using namespace godot;

void GDPureDataHandle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_to_pd_search_paths", "res_path"), &GDPureDataHandle::add_to_pd_search_paths);

	ClassDB::bind_method(D_METHOD("initialize", "in_channels", "out_channels", "sample_rate", "queued"), &GDPureDataHandle::initialize);
	ClassDB::bind_method(D_METHOD("clear"), &GDPureDataHandle::clear);

    ClassDB::bind_method(D_METHOD("open_patch", "path"), &GDPureDataHandle::open_patch);
	ClassDB::bind_method(D_METHOD("close_patch", "patch"), &GDPureDataHandle::close_patch);

	ClassDB::bind_method(D_METHOD("unsubscribe", "source"), &GDPureDataHandle::unsubscribe);
	ClassDB::bind_method(D_METHOD("subscribe", "source"), &GDPureDataHandle::subscribe);
	ClassDB::bind_method(D_METHOD("source_exists", "source"), &GDPureDataHandle::source_exists);
	ClassDB::bind_method(D_METHOD("unsubscribe_all"), &GDPureDataHandle::unsubscribe_all);

	ClassDB::bind_method(D_METHOD("send_bang"), &GDPureDataHandle::send_bang);
	ClassDB::bind_method(D_METHOD("send_float", "receiver", "value"), &GDPureDataHandle::send_float);
	ClassDB::bind_method(D_METHOD("send_symbol", "destination", "symbol"), &GDPureDataHandle::send_symbol);

	ClassDB::bind_method(D_METHOD("start_message"), &GDPureDataHandle::start_message);
	ClassDB::bind_method(D_METHOD("add_float", "value"), &GDPureDataHandle::add_float);
	ClassDB::bind_method(D_METHOD("add_symbol", "symbol"), &GDPureDataHandle::add_symbol);
	ClassDB::bind_method(D_METHOD("finish_list", "destination"), &GDPureDataHandle::finish_list);
	ClassDB::bind_method(D_METHOD("finish_message", "destination", "message"), &GDPureDataHandle::finish_message);

	ClassDB::bind_method(D_METHOD("midi_send_note_on", "channel", "pitch", "velocity"), &GDPureDataHandle::midi_send_note_on);
	ClassDB::bind_method(D_METHOD("midi_send_control_change", "channel", "controller", "value"), &GDPureDataHandle::midi_send_control_change);
	ClassDB::bind_method(D_METHOD("midi_send_program_change", "channel", "value"), &GDPureDataHandle::midi_send_program_change);
	ClassDB::bind_method(D_METHOD("midi_send_pitch_bend", "channel", "value"), &GDPureDataHandle::midi_send_pitch_bend);
	ClassDB::bind_method(D_METHOD("midi_send_aftertouch", "channel", "value"), &GDPureDataHandle::midi_send_aftertouch);
	ClassDB::bind_method(D_METHOD("midi_send_poly_aftertouch", "channel", "pitch", "value"), &GDPureDataHandle::midi_send_poly_aftertouch);

	ClassDB::bind_method(D_METHOD("midi_send_byte", "port", "value"), &GDPureDataHandle::midi_send_byte);
	ClassDB::bind_method(D_METHOD("send_systex", "port", "value"), &GDPureDataHandle::send_systex);
	ClassDB::bind_method(D_METHOD("send_sys_real_time", "port", "value"), &GDPureDataHandle::send_sys_real_time);

	ClassDB::bind_method(D_METHOD("is_initialized"), &GDPureDataHandle::is_initialized);
	ClassDB::bind_method(D_METHOD("is_queued"), &GDPureDataHandle::is_queued);
	ClassDB::bind_method(D_METHOD("block_size"), &GDPureDataHandle::block_size);
	ClassDB::bind_method(D_METHOD("set_max_message_length", "length"), &GDPureDataHandle::set_max_message_length);
	ClassDB::bind_method(D_METHOD("max_message_length"), &GDPureDataHandle::max_message_length);
}


GDPureDataHandle::GDPureDataHandle() {
	//The module is initialized here for preview in the editor
	if(!instance.init(1, 2, 44100, true)) {
		std::cerr << "Could not init pd" << std::endl;
		
		exit(1);
	}

    instance.computeAudio(true);
}

GDPureDataHandle::~GDPureDataHandle() {
}

void GDPureDataHandle::_ready(){
	Ref<AudioStreamGenerator> _stream = memnew(AudioStreamGenerator);
	audiostream = _stream;
	audiostream->set_mix_rate(sample_rate);
    set_stream(audiostream);
}

void GDPureDataHandle::_process(double delta)
{
	if(!is_playing()) 
		return; 
	
	Ref<AudioStreamGeneratorPlayback> p = get_stream_playback();

	if(p.is_valid()) {	
		int nframes = std::min(p->get_frames_available(), 2048);
		
		int ticks = nframes / libpd_blocksize();

		if(instance.processFloat(ticks, inbuf, outbuf))
		{
			for(int i = 0; i < nframes; i++)
			{
				auto v = Vector2(outbuf[i*2], outbuf[i*2+1]);

				v = v.clamp(Vector2(-1, -1), Vector2(1, 1));

				p->push_frame(v);
			}
		}


	}
}

bool GDPureDataHandle::initialize(int in_channels, int out_channels, int sample_rate, bool queued=false){
	return instance.init(in_channels, out_channels, sample_rate, queued);
}

void GDPureDataHandle::clear(){
	instance.clear();
}

void GDPureDataHandle::clear_search_path(){
	instance.clearSearchPath();
}

bool GDPureDataHandle::open_patch(String path){
    String dir = path.get_base_dir();
    dir = dir.replace("res://", "./");

    pd::Patch patch = instance.openPatch(path.get_file().utf8().get_data(), dir.utf8().get_data());

    return patch.isValid();
}

void GDPureDataHandle::close_patch(String patch){
	std::string _patch = std::string(patch.utf8());

	instance.closePatch(_patch);
}

void GDPureDataHandle::add_to_pd_search_paths(String path){
	String _s = path.replace("res://", "./");
	std::string _path = std::string(_s.utf8());

	instance.addToSearchPath(_path);
}


//-- Not exposed to gdscript
bool GDPureDataHandle::process_float(int ticks, float *inBuffer, float *outBuffer){
	return instance.processFloat(ticks, inBuffer, outBuffer);
}

bool GDPureDataHandle::process_short(int ticks, short *inBuffer, short *outBuffer){
	return instance.processShort(ticks, inBuffer, outBuffer);
}

bool GDPureDataHandle::process_double(int ticks, double *inBuffer, double *outBuffer){
	return instance.processDouble(ticks, inBuffer, outBuffer);
}

void GDPureDataHandle::compute_audio(bool state){
	instance.computeAudio(state);
}
//-- Not exposed to gdscript

//-- Receiving Messages
void GDPureDataHandle::subscribe(String source){
	instance.subscribe(std::string(source.utf8()));
}

void GDPureDataHandle::unsubscribe(String source){
	instance.unsubscribe(std::string(source.utf8()));
}

bool GDPureDataHandle::source_exists(String source){
	return instance.exists(std::string(source.utf8()));
}

void GDPureDataHandle::unsubscribe_all(){
	instance.unsubscribeAll();
}
//-- 

//-- Sending Messages
void GDPureDataHandle::send_bang(String receiver){
    std::string _receiver = std::string(receiver.utf8());

    instance.sendBang(_receiver);
}


void GDPureDataHandle::send_float(String receiver, float value){
    std::string _receiver = std::string(receiver.utf8());

    instance.sendFloat(_receiver, value);
}

void GDPureDataHandle::send_symbol(String receiver,String symbol){
    std::string _receiver = std::string(receiver.utf8());
	std::string _symbol = std::string(symbol.utf8());

    instance.sendSymbol(_receiver, _symbol);
}
//--


//-- Sending Compound Messages
void GDPureDataHandle::start_message(){
	instance.startMessage();
}

void GDPureDataHandle::add_float(float value){
	instance.addFloat(value);
}

void GDPureDataHandle::add_symbol(String symbol){
	std::string _symbol = std::string(symbol.utf8());

	instance.addSymbol(_symbol);
}

void GDPureDataHandle::finish_list(String dest){
	std::string _dest = std::string(dest.utf8());

	instance.finishList(_dest);
}

void GDPureDataHandle::finish_message(String dest, String msg){
	std::string _dest = std::string(dest.utf8());
	std::string _msg = std::string(msg.utf8());
	
	instance.finishMessage(_dest, _msg);
}
//--

//--

	//sending midi
void GDPureDataHandle::midi_send_note_on(int channel, int pitch, int velocity = 64){
	instance.sendNoteOn(channel, pitch, velocity);
};

void GDPureDataHandle::midi_send_control_change(int channel, int controller, int value){
	instance.sendControlChange(channel, controller, value);
}

void GDPureDataHandle::midi_send_program_change(int channel, int value){
	instance.sendProgramChange(channel, value);
}

void GDPureDataHandle::midi_send_pitch_bend(int channel, int value){
	instance.sendPitchBend(channel, value);
}

void GDPureDataHandle::midi_send_aftertouch(int channel, int value){
	instance.sendAftertouch(channel, value);
}

void GDPureDataHandle::midi_send_poly_aftertouch(int channel, int pitch, int value){
	instance.sendPolyAftertouch(channel, pitch, value);
}
//

//sending raw_midi_bytes
void GDPureDataHandle::midi_send_byte(int port, int value){
	instance.sendMidiByte(port, value);
}
void GDPureDataHandle::send_systex(int port, int value){
	instance.sendSysex(port, value);
}
void GDPureDataHandle::send_sys_real_time(int port, int value){
	instance.sendSysRealTime(port, value);
}
//