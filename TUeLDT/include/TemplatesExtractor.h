#ifndef TEMPLATEEXTRACTOR_H
#define TEMPLATEEXTRACTOR_H
#include <Eigen/Dense>
#include "State.h"

using namespace Eigen;

class TemplatesExtractor
{
public:
	TemplatesExtractor(const Templates&, Templates&);
	~TemplatesExtractor();

};

#endif // TEMPLATEEXTRACTOR_H
