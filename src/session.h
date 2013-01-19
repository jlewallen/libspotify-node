#ifndef SPOTIFY_SESSION_H_
#define SPOTIFY_SESSION_H_

#include "index.h"
#include "callback_queue.h"
#include "playlistcontainer.h"

#include <queue>

typedef struct audio_fifo {
  TAILQ_HEAD(, audio_fifo_data) q;
  int32_t total_frames;
  uv_async_t async;
  uv_mutex_t mutex;
} audio_fifo_t;

class Session : public EventEmitter {
 public:
  static void Initialize(v8::Handle<v8::Object> target);

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Login(const v8::Arguments& args);
  static v8::Handle<v8::Value> Logout(const v8::Arguments& args);
  static v8::Handle<v8::Value> Search(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetTrackByLink(const v8::Arguments& args);
  static v8::Handle<v8::Value> ConnectionStateGetter(
      v8::Local<v8::String> property,
      const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> PlaylistContainerGetter(
      v8::Local<v8::String> property,
      const v8::AccessorInfo& info);

  // Gets the user associated with a session
  static v8::Handle<v8::Value> UserGetter(v8::Local<v8::String> property,
                                          const v8::AccessorInfo& info);

  void ProcessEvents();
  void Close();

  explicit Session();
  ~Session();

  sp_session* session_;
  pthread_t main_thread_id_;
  v8::Persistent<v8::Function> *logout_callback_;
  v8::Persistent<v8::Function> *login_callback_;
  v8::Persistent<v8::Object> *playlist_container_;

  // Node-Spotify runloop glue
  uv_timer_t runloop_timer_;
  uv_async_t runloop_async_;

  // Spotify background thread-to-node-main glue
  uv_async_t logmsg_async_;

  // Log messages delivered from a background thread
  std::queue<const char*> log_message_queue_;

  // queued callbacks waiting for metadata_update events for particular objects
  CallbackQueue  metadata_update_queue_;

  void DequeueLogMessages();

  audio_fifo_t audio_fifo_;
};

#endif  // SPOTIFY_SESSION_H_
