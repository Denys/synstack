#include "delay.h"

CTSS_DSPNode *ctss_delay(char *id,
                         CTSS_DSPNode *src,
                         uint32_t len,
                         float feedback,
                         uint8_t channels) {
  CTSS_DSPNode *node     = ctss_node(id, channels);
  CTSS_DelayState *state = CTSS_CALLOC(1, sizeof(CTSS_DelayState));
  state->delayLine       = CTSS_CALLOC(len * channels, sizeof(float));
  state->src             = src->buf;
  state->len             = len * channels;
  state->feedback        = feedback;
  state->channels        = channels;
  state->writePos        = 0;
  state->readPos         = channels;
  state->writePtr        = state->delayLine;
  state->readPtr         = state->delayLine + channels;
  node->state            = state;
  node->handler          = ctss_process_delay;
  return node;
}

uint8_t ctss_process_delay(CTSS_DSPNode *node,
                           CTSS_DSPStack *stack,
                           CTSS_Synth *synth) {
  CTSS_UNUSED(synth);
  CTSS_UNUSED(stack);
  CTSS_DelayState *state = node->state;
  float *read            = state->readPtr;
  float *write           = state->writePtr;
  float *src             = state->src;
  float *buf             = node->buf;
  size_t len             = AUDIO_BUFFER_SIZE * state->channels;
  while (len--) {
    *write = (*src++) + (*read++) * state->feedback;
    *buf++ = *write++;
    if (++state->readPos == state->len) {
      state->readPos = 0;
      read           = state->delayLine;
    }
    if (++state->writePos == state->len) {
      state->writePos = 0;
      write           = state->delayLine;
    }
  }
  state->readPtr  = read;
  state->writePtr = write;
  return 0;
}
