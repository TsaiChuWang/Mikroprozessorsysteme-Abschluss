#define SIAMESE
#ifdef SIAMESE
#define __type 1296
#define HIDDEN_SIZE 5
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01
#define EPOCHS 1000

typedef struct {
    double weights[__type][HIDDEN_SIZE];
    double bias[HIDDEN_SIZE];
} NeuralNetwork;

typedef struct {
    double input[__type];
} Sample;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

void initializeNetwork(NeuralNetwork *network) {
    // Initialize weights and biases
    for (int i = 0; i < __type; ++i) {
        for (int j = 0; j < HIDDEN_SIZE; ++j) {
            network->weights[i][j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        }
    }

    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        network->bias[i] = 0.0;
    }
}

double forwardPass(NeuralNetwork *network, double input[]) {
    // Forward pass through the network
    double hiddenLayer[HIDDEN_SIZE];

    for (int j = 0; j < HIDDEN_SIZE; ++j) {
        hiddenLayer[j] = 0.0;
        for (int i = 0; i < __type; ++i) {
            hiddenLayer[j] += input[i] * network->weights[i][j];
        }
        hiddenLayer[j] += network->bias[j];
        hiddenLayer[j] = sigmoid(hiddenLayer[j]);
    }

    double output = 0.0;
    for (int j = 0; j < HIDDEN_SIZE; ++j) {
        output += hiddenLayer[j];
    }

    return sigmoid(output);
}

void trainSiameseNetwork(NeuralNetwork *network, Sample samples[]) {
    // Training the Siamese network
    for (int epoch = 0; epoch < EPOCHS; ++epoch) {
        for (int i = 0; i < 2; ++i) {
            int anchorIdx = rand() % 10;  // Index for the anchor sample
            int positiveIdx = rand() % 10;  // Index for the positive sample
            int negativeIdx = rand() % 10;  // Index for the negative sample

            double anchorOutput = forwardPass(network, samples[anchorIdx].input);
            double positiveOutput = forwardPass(network, samples[positiveIdx].input);
            double negativeOutput = forwardPass(network, samples[negativeIdx].input);

            // Update weights and biases
            for (int j = 0; j < HIDDEN_SIZE; ++j) {
                double dw = LEARNING_RATE * (positiveOutput - anchorOutput) * samples[anchorIdx].input[j];
                network->weights[j] += dw;

                dw = LEARNING_RATE * (anchorOutput - negativeOutput) * samples[negativeIdx].input[j];
                network->weights[j] += dw;

                network->bias[j] += LEARNING_RATE * (positiveOutput - anchorOutput + anchorOutput - negativeOutput);
            }
        }
    }
}

#endif