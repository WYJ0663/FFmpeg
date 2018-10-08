////
//// Created by Ezon Wu on 2018/10/6.
////
//
//#include <android/log.h>
//#include <android/native_window_jni.h>
//#include <android/native_window.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <pthread.h>
////解码
//#include "libavcodec/avcodec.h"
////封装格式处理
//#include "libavformat/avformat.h"
////像素处理
//#include "libswscale/swscale.h"
//
//#include "libavutil/imgutils.h"
//#include "libavutil/time.h"
////重采样
//#include "libswresample/swresample.h"
//
//#include "libavfilter/avfilter.h"
//#include "libavfilter/buffersink.h"
//#include "libavfilter/buffersrc.h"
//
//// for native audio
//#include <SLES/OpenSLES.h>
//#include <SLES/OpenSLES_Android.h>
//
//#include "player.h"
//
////音频解码 采样率 新版版可达48000 * 4
//#define MAX_AUDIO_FRME_SIZE  2 * 44100
//#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
//
//#define  LOG_TAG    "yijun_player"
//#define  LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,FORMAT,##__VA_ARGS__);
//#define  LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,FORMAT,##__VA_ARGS__);
//#define  LOGD(FORMAT, ...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,FORMAT, ##__VA_ARGS__)
//
//typedef int boolean;
//#define true 1
//#define false 0
//
//typedef struct _Player Player;
//
//struct _Player {
//    char *url;//播放连接
//
//    boolean isPlay;
//
//    //封装格式上下文
//    AVFormatContext *input_format_ctx;
//    //音频视频流索引位置
//    int video_stream_index;
//    int audio_stream_index;
//    //流的总个数
//    int capture_streams_no;
//
//    //解密器
//    AVCodecContext *video_codec_ctx;
//    AVCodecContext *audio_codec_ctx;
//
//    /**
//     * 音频信息
//     */
//    SwrContext *swr_ctx;
//    //输入的采样格式
//    enum AVSampleFormat in_sample_fmt;
//    //输出采样格式16bit PCM
//    enum AVSampleFormat out_sample_fmt;
//    //输入采样率
//    int in_sample_rate;
//    //输出采样率
//    int out_sample_rate;
//    //输出的声道个数
//    int out_channel_nb;
//
//    SLObjectItf engineObject;
//    SLEngineItf engineEngine;
//    // output mix interfaces
//    SLObjectItf outputMixObject = NULL;
//    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
//// buffer queue player interfaces
//    static SLObjectItf bqPlayerObject = NULL;
//    static SLPlayItf bqPlayerPlay;
//    static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
//    static SLEffectSendItf bqPlayerEffectSend;
//    static SLVolumeItf bqPlayerVolume;
//    static uint8_t decoded_audio_buf[AVCODEC_MAX_AUDIO_FRAME_SIZE];
//
//    /**
//     * 视频信息
//     */
//};
//
////获取解码器
//boolean find_and_open_codec(AVCodecContext *codec_ctx) {
//    LOGI("init_codec_context begin");
//    AVCodec *pCodec = avcodec_find_decoder(codec_ctx->codec_id);
//    if (pCodec == NULL) {
//        LOGD("video Codec not found.");
//        return false; // Codec not found
//    }
//    if (avcodec_open2(codec_ctx, pCodec, NULL) < 0) {
//        LOGD("video Could not open codec.");
//        return false; // Could not open codec
//    }
//
//    return true;
//}
//
////音频解码准备
//void decode_audio_prepare(Player *player) {
//    LOGD("decode_audio_prepare");
//
//    AVCodecContext *codec_ctx = player->audio_codec_ctx;
//    //frame->16bit  44100 PCM 统一音频采样格式与采样率
//    SwrContext *swr_ctx = swr_alloc();
//    //输入采样率格式
//    enum AVSampleFormat in_sample_fmt = codec_ctx->sample_fmt;
//    //输出采样率格式16bit PCM
//    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
//    //输入采样率
//    int in_sample_rate = codec_ctx->sample_rate;
//    //输出采样率
//    int out_sample_rate = 44100;
//    //获取输入的声道布局
//    //根据声道个数获取默认的声道布局(2个声道，默认立体声)
//    //av_get_default_channel_layout(pCodeCtx->channels);
//    uint64_t in_ch_layout = codec_ctx->channel_layout;
//    //输出的声道布局
//    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
//
//
//    swr_alloc_set_opts(swr_ctx, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout, in_sample_fmt,
//                       in_sample_rate, 0, NULL);
//
//    swr_init(swr_ctx);
//
//    //获取输入输出的声道个数
//    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
//    LOGI("out_count:%d", out_channel_nb);
//
//    //重采样设置参数-------------end
//
//    player->in_sample_fmt = in_sample_fmt;
//    player->out_sample_fmt = out_sample_fmt;
//    player->in_sample_rate = in_sample_rate;
//    player->out_sample_rate = out_sample_rate;
//    player->out_channel_nb = out_channel_nb;
//    player->swr_ctx = swr_ctx;
//}
//
//
///**
//* 音频解码准备
//*/
//double audioClock;
//
//void decode_audio(Player *player, AVPacket *packet, uint8_t *buffer) {
//
//    AVFormatContext *input_format_ctx = player->input_format_ctx;
//    AVStream *stream = input_format_ctx->streams[player->video_stream_index];
//    AVCodecContext *codec_ctx = player->audio_codec_ctx;
//    LOGI("%s", "decode_audio");
//    //解压缩数据
//    AVFrame *frame = av_frame_alloc();
//    int got_frame;
//    avcodec_decode_audio4(codec_ctx, frame, &got_frame, packet);
//    //16bit 44100 PCM 数据（重采样缓冲区）
//
//    //非0，正在解码
//    if (got_frame > 0) {
//        swr_convert(player->swr_ctx, &buffer, MAX_AUDIO_FRME_SIZE, (const uint8_t **) frame->data, frame->nb_samples);
//        //获取sample的size
//        int out_buffer_size;
//
//        if (player->out_sample_fmt == AV_SAMPLE_FMT_S16P) {
//            out_buffer_size = av_samples_get_buffer_size(frame->linesize, player->out_channel_nb, frame->nb_samples,
//                                                         player->out_sample_fmt, 1);
//        } else {
//            av_samples_get_buffer_size(&out_buffer_size, player->out_channel_nb, frame->nb_samples,
//                                       player->out_sample_fmt, 1);
//        }
//
//
//        audioClock = frame->pkt_pts * av_q2d(stream->time_base);
//
//        //关联当前线程的JNIEnv
//        JavaVM *javaVM = player->javaVM;
//        JNIEnv *env;
//        (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
//        //out_buffer 缓冲区数据，转换成byte数组
//        jbyteArray audio_sample_array = (*env)->NewByteArray(env, out_buffer_size);
//
//        jbyte *sample_byte = (*env)->GetByteArrayElements(env, audio_sample_array, NULL);
//
//        //将out_buffer的数据复制到sample_byte
//        memcpy(sample_byte, buffer, out_buffer_size);
//
//        //同步数据 同时释放sample_byte
//        (*env)->ReleaseByteArrayElements(env, audio_sample_array, sample_byte, 0);
//
//        //AudioTrack.write PCM数据
//        (*env)->CallIntMethod(env, player->audio_track, player->audio_track_write_mid, audio_sample_array, 0,
//                              out_buffer_size);
//
//        //释放局部引用  否则报错JNI ERROR (app bug): local reference table overflow (max=512)
//        (*env)->DeleteLocalRef(env, audio_sample_array);
//        (*javaVM)->DetachCurrentThread(javaVM);
//
//    }
//
//    av_frame_free(&frame);
//    //av_free(&out_buffer);
//}
//
//int createEngine(Player *player) {
//
//    SLresult result;
//
//    // create engine
//    result = slCreateEngine(&(player->engineObject), 0, NULL, 0, NULL, NULL);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("slCreateEngine failure.");
//        return -1;
//    }
//
//    // realize the engine
//    result = (*(player->engineObject))->Realize((player->engineObject), SL_BOOLEAN_FALSE);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("(player->engineObject) Realize failure.");
//        (*(player->engineObject))->Destroy((player->engineObject));
//        (player->engineObject) = NULL;
//        (player->engineEngine) = NULL;
//        return -1;
//    }
//
//    // get the engine interface, which is needed in order to create other objects
//    result = (*(player->engineObject))->GetInterface((player->engineObject), SL_IID_ENGINE,
//                                                     &(player->engineEngine));
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("(player->engineObject) GetInterface failure.");
//        (*(player->engineObject))->Destroy((player->engineObject));
//        (player->engineObject) = NULL;
//        (player->engineEngine) = NULL;
//        return -1;
//    }
//
//    // create output mix, with environmental reverb specified as a non-required interface
//    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
//    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
//    result = (*(player->engineEngine))->CreateOutputMix((player->engineEngine), &(player->outputMixObject), 1,
//                                                        ids, req);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("(player->engineObject) CreateOutputMix failure.");
//        (*(player->engineObject))->Destroy((player->engineObject));
//        (player->engineObject) = NULL;
//        (player->engineEngine) = NULL;
//        return -1;
//    }
//
//    // realize the output mix
//    result = (*(player->outputMixObject))->Realize(player->outputMixObject, SL_BOOLEAN_FALSE);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("outputMixObject Realize failure.");
//
//        (*(player->outputMixObject))->Destroy(player->outputMixObject);
//        player->outputMixObject = NULL;
//        player->outputMixEnvironmentalReverb = NULL;
//        (*(player->engineObject))->Destroy(player->engineObject);
//        player->engineObject = NULL;
//        (player->engineEngine) = NULL;
//        return -1;
//    }
//
//    // get the environmental reverb interface
//    // this could fail if the environmental reverb effect is not available,
//    // either because the feature is not present, excessive CPU load, or
//    // the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
//    result = (*(player->outputMixObject))->GetInterface(player->outputMixObject,
//                                                        SL_IID_ENVIRONMENTALREVERB,
//                                                        &(player->outputMixEnvironmentalReverb));
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("outputMixObject Realize failure.");
//        (*(player->outputMixObject))->Destroy((player->outputMixObject));
//        player->outputMixObject = NULL;
//        player->outputMixEnvironmentalReverb = NULL;
//        (*(player->engineObject))->Destroy(player->engineObject);
//        player->engineObject = NULL;
//        player->engineEngine = NULL;
//        return -1;
//    }
//
//    LOGE("OpenSL ES createEngine success.");
//    return 0;
//}
//
//void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
//    Player *player = (Player *) context;
//
//    SLresult result;
//
//    //LOGV2("bqPlayerCallback...");
//
//    if (bq != player->bqPlayerBufferQueue) {
//        LOGE("bqPlayerCallback : not the same player object.");
//        return;
//    }
//
//    int decoded_size = audio_decode_frame(player, player->decoded_audio_buf,
//                                          sizeof(player->decoded_audio_buf));
//    if (decoded_size > 0) {
//        result = (*(player->bqPlayerBufferQueue))->Enqueue(player->bqPlayerBufferQueue,
//                                                 player->decoded_audio_buf, decoded_size);
//        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
//        // which for this code example would indicate a programming error
//        if (SL_RESULT_SUCCESS != result) {
//            LOGE("bqPlayerCallback : bqPlayerBufferQueue Enqueue failure.");
//        }
//    }
//}
//
//static inline int64_t get_valid_channel_layout(int64_t channel_layout,
//                                               int channels) {
//    if (channel_layout
//        && av_get_channel_layout_nb_channels(channel_layout) == channels) {
//        return channel_layout;
//    } else {
//        return 0;
//    }
//}
//
//// decode a new packet(not multi-frame)
//// return decoded frame size, not decoded packet size
//int audio_decode_frame(Player *player, uint8_t *audio_buf, int buf_size) {
//    static AVPacket pkt;
//    static uint8_t *audio_pkt_data = NULL;
//    static int audio_pkt_size = 0;
//    int len1, data_size;
//    int got_frame;
//    AVFrame * frame = NULL;
//    static int reconfigure = 1;
//    int ret = -1;
//
//    for (;;) {
//
//        while (audio_pkt_size > 0) {
//
//            if (NULL == frame) {
//                frame = av_frame_alloc();
//            }
//
//            data_size = buf_size;
//            got_frame = 0;
//
//            // len1 is decoded packet size
//            len1 = avcodec_decode_audio4(player->audio_codec_ctx, frame,
//                                         &got_frame, &pkt);
//            if (got_frame) {
//
//                if (reconfigure) {
//
//                    reconfigure = 0;
//                    int64_t dec_channel_layout = get_valid_channel_layout(
//                            frame->channel_layout,
//                            av_frame_get_channels(frame));
//
//                    // used by init_filter_graph()
//                    audio_filter_src.fmt = (enum AVSampleFormat) frame->format;
//                    audio_filter_src.channels = av_frame_get_channels(frame);
//                    audio_filter_src.channel_layout = dec_channel_layout;
//                    audio_filter_src.freq = frame->sample_rate;
//
//                    init_filter_graph(&agraph, &in_audio_filter,
//                                      &out_audio_filter);
//                }
//
//                if ((ret = av_buffersrc_add_frame(in_audio_filter, frame))
//                    < 0) {
//                    av_log(NULL, AV_LOG_ERROR,
//                           "av_buffersrc_add_frame :  failure. \n");
//                    return ret;
//                }
//
//                if ((ret = av_buffersink_get_frame(out_audio_filter, frame))
//                    < 0) {
//                    av_log(NULL, AV_LOG_ERROR,
//                           "av_buffersink_get_frame :  failure. \n");
//                    continue;
//                }
//
//                data_size = av_samples_get_buffer_size(NULL, frame->channels,
//                                                       frame->nb_samples, (enum AVSampleFormat) frame->format,
//                                                       1);
//
//                // len1 is decoded packet size
//                // < 0  means failure or error，so break to get a new packet
//                if (len1 < 0) {
//                    audio_pkt_size = 0;
//                    av_log(NULL, AV_LOG_ERROR,
//                           "avcodec_decode_audio4 failure. \n");
//                    break;
//                }
//
//                // decoded data to audio buf
//                memcpy(audio_buf, frame->data[0], data_size);
//
//                audio_pkt_data += len1;
//                audio_pkt_size -= len1;
//
//                int n = 2 * global_context.acodec_ctx->channels;
//                /*audio_clock += (double) data_size
//                 / (double) (n * global_context.acodec_ctx->sample_rate); // add bytes offset */
//                av_free_packet(&pkt);
//                av_frame_free(&frame);
//
//                return data_size;
//            } else if (len1 < 0) {
//                char errbuf[64];
//                av_strerror(ret, errbuf, 64);
//                LOGV2("avcodec_decode_audio4 ret < 0, %s", errbuf);
//            }
//        }
//
//        av_free_packet(&pkt);
//        av_frame_free(&frame);
//
//        // get a new packet
//        if (packet_queue_get(&global_context.audio_queue, &pkt) < 0) {
//            return -1;
//        }
//
//        //LOGV2("pkt.size is %d", pkt.size);
//
//        audio_pkt_data = pkt.data;
//        audio_pkt_size = pkt.size;
//    }
//
//    return ret;
//}
//
//
//int createBufferQueueAudioPlayer(Player *player) {
//    SLresult result;
//    SLuint32 channelMask;
//
//    // configure audio source
//    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
//            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
//
//    if (player->audio_codec_ctx->channels == 2)
//        channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
//    else
//        channelMask = SL_SPEAKER_FRONT_CENTER;
//
//    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM,
//                                   player->audio_codec_ctx->channels,
//                                   player->audio_codec_ctx->sample_rate * 1000,
//                                   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
//                                   channelMask, SL_BYTEORDER_LITTLEENDIAN};
//
//    SLDataSource audioSrc = {&loc_bufq, &format_pcm};
//
//    // configure audio sink
//    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX,
//                                          player->outputMixObject};
//    SLDataSink audioSnk = {&loc_outmix, NULL};
//
//    // create audio player
//    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
//                                  SL_IID_VOLUME};
//    const SLboolean req[3] =
//            {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
//    result = (*(player->engineEngine))->CreateAudioPlayer(player->engineEngine, &(player->bqPlayerObject),
//                                                          &audioSrc, &audioSnk, 3, ids, req);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("CreateAudioPlayer failure.");
//        return -1;
//    }
//
//    // realize the player
//    result = (*(player->bqPlayerObject))->Realize(player->bqPlayerObject, SL_BOOLEAN_FALSE);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("bqPlayerObject Realize failure.");
//        return -1;
//    }
//
//    // get the play interface
//    result = (*(player->bqPlayerObject))->GetInterface(player->bqPlayerObject, SL_IID_PLAY,
//                                                       &(player->bqPlayerPlay));
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("bqPlayerObject GetInterface failure.");
//        return -1;
//    }
//
//    // get the buffer queue interface
//    result = (*(player->bqPlayerObject))->GetInterface(player->bqPlayerObject, SL_IID_BUFFERQUEUE,
//                                                       &(player->bqPlayerBufferQueue));
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("bqPlayerObject GetInterface failure.");
//        return -1;
//    }
//
//    // register callback on the buffer queue
//    result = (*(player->bqPlayerBufferQueue))->RegisterCallback(player->bqPlayerBufferQueue,
//                                                                bqPlayerCallback, player);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("bqPlayerObject RegisterCallback failure.");
//        return -1;
//    }
//
//    // get the effect send interface
//    result = (*(player->bqPlayerObject))->GetInterface(player->bqPlayerObject, SL_IID_EFFECTSEND,
//                                                       &(player->bqPlayerEffectSend));
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("bqPlayerObject GetInterface SL_IID_EFFECTSEND failure.");
//        return -1;
//    }
//
//    // get the volume interface
//    result = (*(player->bqPlayerObject))->GetInterface(player->bqPlayerObject, SL_IID_VOLUME,
//                                                       &(player->bqPlayerVolume));
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("bqPlayerObject GetInterface SL_IID_VOLUME failure.");
//        return -1;
//    }
//
//    // set the player's state to playing
//    result = (*(player->bqPlayerPlay))->SetPlayState(player->bqPlayerPlay, SL_PLAYSTATE_PLAYING);
//    if (SL_RESULT_SUCCESS != result) {
//        LOGE("bqPlayerObject SetPlayState SL_PLAYSTATE_PLAYING failure.");
//        return -1;
//    }
//
//    LOGE("OpenSL ES CreateAudioPlayer success.");
//
//    return 0;
//}
//
//
///**
//* 解码子线程函数
//*/
//void audio_decode_data(Player *player, AVPacket packet) {
//    int stream_index = player->audio_stream_index;
//
//    AVFormatContext *format_ctx = player->input_format_ctx;
//    int video_frame_count = 0, audio_frame_count = 0;
//
//    uint8_t *buffer;
//    buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRME_SIZE);
//
//
//    while (&player->isPlay) {
//
//        decode_audio(player, &packet, buffer);
//        LOGI("audio_frame_count:%d", audio_frame_count++);
//        LOGI("pthread_mutex_unlock  AVPacket");
//    }
//    av_free(buffer);
//}
//
//void player_read_from_stream(Player *player) {
//    int index = 0;
//    LOGI("player_read_from_stream start");
//    int ret;
//    //栈内存上保存一个AVPacket
//    AVPacket packet;
//
//    while (true) {
//        ret = av_read_frame(player->input_format_ctx, &packet);
//        LOGI("player_read_from_stream :%d", index++);
//        //到文件结尾
//        if (ret < 0) {
//            break;
//        }
//
//
//        if (packet.stream_index == player->audio_stream_index) {
//            audio_decode_data(player, packet);
////            packet.stream_index == player->video_stream_index ||
////            pthread_mutex_lock(&player->mutex);
////            QueuePush(player->packets[packet.stream_index], packet, &player->mutex, &player->cond, 0);
////            pthread_mutex_unlock(&player->mutex);
//        }
//
//        LOGI("pthread_mutex_unlock  player_read_from_stream");
//    }
//
//
//}
//
//boolean open_av(Player *player) {
//    LOGD("open_av");
//    av_register_all();
//
//    AVFormatContext *format_ctx = avformat_alloc_context();
//
//    // Open video file
//    if (avformat_open_input(&format_ctx, player->url, NULL, NULL) != 0) {
//        LOGD("Couldn't open file:%s\n", player->url);
//        return false; // Couldn't open file
//    }
//
//    // Retrieve stream information
//    if (avformat_find_stream_info(format_ctx, NULL) < 0) {
//        LOGD("Couldn't find stream information.");
//        return false;
//    }
//
//    player->capture_streams_no = format_ctx->nb_streams;
//    LOGI("captrue_streams_no:%d", player->capture_streams_no);
//    // Find the first video stream
//    //获取音频和视频流的不同索引位置
//    int i;
//    for (i = 0; i < format_ctx->nb_streams; i++) {
//        AVCodecContext *codec_ctx = format_ctx->streams[i]->codec;
//        LOGI("codec_type:%d", i);
//
//        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
//            if (!find_and_open_codec(codec_ctx)) {
//                return false;
//            }
//            player->video_stream_index = i;
//            player->video_codec_ctx = codec_ctx;
//        } else if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
//            if (!find_and_open_codec(codec_ctx)) {
//                return false;
//            }
//            player->audio_stream_index = i;
//            player->audio_codec_ctx = codec_ctx;
//        }
//    }
//
//    player->input_format_ctx = format_ctx;
//    LOGD("open_av end");
//    return true;
//}
//
//
//void *_player(void *arg) {
//    Player *player = (Player *) arg;
//
//    if (!open_av(player)) {
//        return 0;
//    }
//
//
//    decode_audio_prepare(player);
//
//    player_read_from_stream(player);
//
//    // Close the video file
//    avformat_close_input(&player->input_format_ctx);
//    free(player);
//}
//
//JNIEXPORT void JNICALL Java_com_example_ffmpeg_MyPlayActivity_play
//        (JNIEnv *env, jclass type, jstring input_str, jobject surface) {
//    const char *url = (*env)->GetStringUTFChars(env, input_str, NULL);
//    Player *player = (Player *) malloc(sizeof(Player));
//    player->url = (char *) url;
//    player->isPlay = true;
//
//    pthread_t player_thread;
//    pthread_create(&player_thread, NULL, _player, (void *) player);
//
////    _player(player);
////    (*env)->ReleaseStringUTFChars(env, input_str, url);
//}
