/* forward algoritm: return observation likelihood */
float forward(int *data, int len, int nstates, int nobvs,
        float *prior, float * trans, float *obvs)
{
    /* construct trellis */
    // float alpha[len][nstates];
    // float beta[len][nstates];
    float *alpha = (float *)malloc(len * nstates * sizeof(float));
    float *beta = (float *)malloc(len * nstates * sizeof(float));

    float loglik;

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < nstates; j++) {
            alpha[i * nstates + j] = - INFINITY;
            beta[i * nstates + j] = - INFINITY;
        }
    }

    double startTime = CycleTimer::currentSeconds();
    /* forward pass */
    for (int i = 0; i < nstates; i++) {
        alpha[i] = prior[i] + obvs[IDX(i,data[0],nobvs)];
    }

    for (int i = 1; i < len; i++) {
        for (int j = 0; j < nstates; j++) {
            for (int k = 0; k < nstates; k++) {
                float p = alpha[(i-1) * nstates + k] + trans[IDXT(k,j,nstates)] + obvs[IDX(j,data[i],nobvs)];
                alpha[i * nstates + j] = logadd(alpha[i * nstates + j], p);
            }
        }
    }

    loglik = -INFINITY;
    for (int i = 0; i < nstates; i++) {
        loglik = logadd(loglik, alpha[(len-1) * nstates + i]);
    }
    double endTime = CycleTimer::currentSeconds();
    printf("Time taken %.4f milliseconds\n",  (endTime - startTime) * 1000);

    return loglik;
}