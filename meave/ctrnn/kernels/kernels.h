#ifndef MEAVE_CTRNN_KERNELS_KERNSLS_H
#	define MEAVE_CTRNN_KERNELS_KERNSLS_H

void meave_ctrnn_nn_val_avx2_kernel(const unsigned neurons_num, const unsigned networks_num,
				    const float *p_time_steps, const float *p_y, const float *p_tc, const float *p_ei, const float *p_w, float *p_v);
void meave_ctrnn_nn_sigm_avx2_kernel();

#endif // MEAVE_CTRNN_KERNELS_KERNSLS_H

