#pragma once

#include <QThread>
#include <QString>
#include "Agent/HumanHand.h"
#include "Sampler.h"

#include "Stats.h"

struct Observation
{
	double weight;
	HumanHand * hand;

	Observation() { weight = 0.0; hand = NULL; }

	inline void set(double fromWeight, HumanHand * fromHand)
	{
		this->weight = fromWeight;
		this->hand = fromHand;
	}
};

class Experiment : public QThread
{
	Q_OBJECT

private:

	Mesh * mesh;

	int numSamples;
	double farThreshold;

	FeatureSpace space;

	bool isSnapshots;
	bool isOutFile;
	QString fileName;

public:

	Experiment();

	void init(Mesh * src_mesh, int numberSamples, double threshold, bool screenshots = false, 
		QString outputFile = "", FeatureSpace usingFeatureSpace = ANGLE, 
		SamplingMethod samplingMethod = RANDOM_BARYCENTRIC);

	Vector<Observation*> obs;

	Sampler * sampler;
	SamplingMethod sampleMethod;

	bool isDone;
	Stats totalTime;

	void forceRun();

	static bool samplePoint(const SamplePoint & sp, HumanHand * h, Mesh * mesh, double far_threshold);

signals:
	void saveSnapshot();
	void updateViewer();
	void addObservation();

protected:
	void run();
};
