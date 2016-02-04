#include "demo_common.h"

const uint8_t scale[] = {36, 40, 43, 45, 60, 48, 52, 55};
const uint8_t scale2[] = {33, 31, 26, 31, 29, 33, 31, 29};
const float pitch[] = {0.5f, 1.0f, 0.25f};

static void init_voice(CT_Synth *synth, CT_DSPStack *stack, float freq);
static int render_synth(const void *in, void *out, unsigned long frames,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags status, void *data);

int main(int argc, char *argv[]) {
    AppState app;
    srand(time(0));
    ct_synth_init(&app.synth, 32);
    app.synth.lfo[0] = ct_synth_osc("lfo1", ct_synth_process_osc_sin, 0.0f,
                                    HZ_TO_RAD(0.125f), 0.5f, 1.2f);
    app.synth.lfo[1] = ct_synth_osc("lfo2", ct_synth_process_osc_sin, 0.0f,
                                    HZ_TO_RAD(0.25f), 0.495f, 0.5f);
    app.synth.lfo[2] = ct_synth_osc("lfo3", ct_synth_process_osc_sin, 0.0f,
                                    HZ_TO_RAD(0.1f), 1.5f, 2.0f);
    app.synth.numLFO = 3;
    for (uint8_t i = 0; i < app.synth.numStacks; i++) {
        init_voice(&app.synth, &app.synth.stacks[i], ct_synth_notes[scale[0]]);
    }
    app.pitch = -8;
    app.callback = render_synth;
    app.handler = demo_key_handler;
    app.channels = 2;
    return demo(&app);
}

static void init_voice(CT_Synth *synth, CT_DSPStack *stack, float freq) {
    ct_synth_init_stack(stack);
    CT_DSPNode *env =
        ct_synth_adsr("env", synth->lfo[0], 0.005f, 0.1f, 0.3f, 1.0f, 0.5f);
    CT_DSPNode *osc1 = ct_synth_osc("osc1", ct_synth_process_osc_spiral, 0.0f,
                                    freq, 0.15f, 0.0f);
    ct_synth_set_osc_lfo(osc1, synth->lfo[2], 1.0f);
    CT_DSPNode *osc2 = ct_synth_osc("osc2", ct_synth_process_osc_spiral, 0.0f,
                                    freq, 0.15f, 0.0f);
    ct_synth_set_osc_lfo(osc2, synth->lfo[2], 1.0f);
    CT_DSPNode *sum =
        ct_synth_op4("sum", osc1, env, osc2, env, ct_synth_process_madd);
    CT_DSPNode *filter =
        ct_synth_filter_iir("filter", IIR_LP, sum, NULL, 11800.0f, 0.0f);
    CT_DSPNode *pan = ct_synth_panning("pan", filter, NULL, 0.0f);
    CT_DSPNode *delay = ct_synth_delay(
        "delay", pan, (uint32_t)(SAMPLE_RATE * 0.25f * 1.5f), 0.6f, 2);

    CT_DSPNode *nodes[] = {env, osc1, osc2, sum, filter, pan, delay};
    ct_synth_build_stack(stack, nodes, 7);
}

static int render_synth(const void *in, void *out, unsigned long frames,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags status, void *data) {
    AppState *app = (AppState *)data;
    float time = (float)timeInfo->currentTime;
    float ft = fmodf(time, 1.0f);
    if (ft < 0.008) {
        float freq = ct_synth_notes[scale[app->noteID % 8] + app->pitch];
        CT_DSPStack *s = &app->synth.stacks[app->voiceID];
        ct_synth_reset_adsr(NODE_ID(s, "env"));
        freq *= pitch[rand() % 3];
        NODE_ID_STATE(CT_OscState, s, "osc1")->freq = HZ_TO_RAD(freq);
        NODE_ID_STATE(CT_OscState, s, "osc2")->freq = HZ_TO_RAD(freq * 1.01f);
        NODE_ID_STATE(CT_PanningState, s, "pan")->pos = ct_randf(0.0f, 1.0f);
        ct_synth_activate_stack(s);
        app->noteID++;
        app->voiceID = (app->voiceID + 1) % app->synth.numStacks;
    }
    ft = fmodf(time, 0.25f);
    if (ft < 0.008) {
        float freq = ct_synth_notes[scale2[app->noteID % 8] + app->pitch + 7];
        CT_DSPStack *s = &app->synth.stacks[app->voiceID];
        ct_synth_reset_adsr(NODE_ID(s, "env"));
        NODE_ID_STATE(CT_OscState, s, "osc1")->freq = HZ_TO_RAD(freq);
        NODE_ID_STATE(CT_OscState, s, "osc2")->freq = HZ_TO_RAD(freq * 1.01f);
        NODE_ID_STATE(CT_PanningState, s, "pan")->pos = ct_randf(0.0f, 1.0f);
        ct_synth_activate_stack(s);
        app->noteID++;
        app->voiceID = (app->voiceID + 1) % app->synth.numStacks;
    }

    ct_synth_update_mix_stereo_f32(&app->synth, frames, (float *)out);
    return 0;
}
