#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <limits.h>

#include "mzSample.h"
#include "mzMassSlicer.h"
#include "options.h"
#include "../mzroll/classifierNeuralNet.h"
#include "pugixml.hpp"
#include "../libmaven/PeakDetector.h"

using namespace std;

//variables
ClassifierNeuralNet clsf;
vector<mzSample*> samples;
vector<Compound*> compoundsDB;
vector<PeakGroup> allgroups;
MassCalculator mcalc;

time_t startTime, curTime, stopTime;

vector<string> filenames;

bool alignSamplesFlag = false;
bool processMassSlicesFlag = true;
bool reduceGroupsFlag = true;
bool matchRtFlag = true;
bool writeReportFlag = true;
bool checkConvergance = true;

string ligandDbFilename;
string clsfModelFilename = "default.model";
string outputdir = "reports" + string(DIR_SEPARATOR_STR);

int ionizationMode = -1;

//mass slice detection
int rtStepSize = 10;
float ppmMerge = 30;
float avgScanTime = 0.2;

//peak detection
int eic_smoothingWindow = 5;

//peak grouping across samples
float grouping_maxRtWindow = 0.5;

//peak filtering criteria
int minGoodPeakCount = 1;
float minSignalBlankRatio = 2;
int minNoNoiseObs = 3;
float minSignalBaseLineRatio = 2;
float minGroupIntensity = 100;
float minQuality = 0.5;

/**
 * [process command line Options]
 * @param argc [argument counter]
 * @param argv [argument variables]
 */
void processOptions(int argc, char* argv[]);

/**
 * [loadSamples description]
 * @param filenames [description]
 */
void loadSamples(vector<string>&filenames);

/**
 * [loadCompoundCSVFile description]
 * @param filename [description]
 */
void loadCompoundCSVFile(string filename);


// void printSettings();

/**
 * [reduce number of Groups]
 */
void reduceGroups();

/**
 * [write multiple types of Reports]
 * @param setName [name of the set]
 */
void writeReport(string setName);

/**
 * [write Sample List in XML]
 * @param parent [parent node]
 */
void writeSampleListXML(xml_node& parent);

/**
 * [write Peak Table in XML]
 * @param filename [name of the file]
 */
void writePeakTableXML(string filename);

/**
 * [write CSV Report]
 * @param filename [name of the file]
 */
void writeCSVReport(string filename);

/**
 * [write Group information in XML]
 * @param parent [parent ion]
 * @param g      [peak group]
 */
void writeGroupXML(xml_node& parent, PeakGroup* g);

/**
 * [write Parameters of ion in XML]
 * @param parent [parent ion]
 */
void writeParametersXML(xml_node& parent);
// bool addPeakGroup(PeakGroup& grp);

vector<mzSlice*> getSrmSlices();

/**
 * [clean Sample Name]
 * @param  sampleName [name of the sample]
 * @return            [cleaned name]
 */
string cleanSampleName(string sampleName);

/**
 * [save EICs]
 * @param filename [name of the file]
 */
void saveEICs(string filename);

/**
 * [save EIC]
 * @param out [the output file]
 * @param eic [the eic to be saved]
 */
void saveEIC(ofstream& out, EIC* eic);

/**
 * [get EICs from a group]
 * @param rtmin [minimum retention time]
 * @param rtmax [maximum retention time]
 * @param grp [the peak group]
 */
vector<EIC*> getEICs(float rtmin, float rtmax, PeakGroup& grp);

PeakDetector* peakDetector;

int main(int argc, char *argv[]) {

	peakDetector = new PeakDetector();

	cout << "read command line options" << endl;
	processOptions(argc, argv);

	cout << "Loading classifiation model" << endl;
	clsf.loadModel(clsfModelFilename);

	cout << "load compound list" << endl;
	if (!peakDetector->ligandDbFilename.empty()) {
		peakDetector->processMassSlicesFlag = false;
		cerr << "Loading ligand database" << endl;
		loadCompoundCSVFile(peakDetector->ligandDbFilename);
	}

	//load files
	loadSamples(filenames);
	if (peakDetector->samples.size() == 0) {
		cerr << "Exiting .. nothing to process " << endl;
		exit(1);
	}

	//get retention time resolution
	peakDetector->setAverageScanTime();

	//ionization
	peakDetector->setIonizationMode();

	//align samples
	peakDetector->alignSamples();

	//process compound list
	if (peakDetector->compounds.size()) {
		vector<mzSlice*> slices = peakDetector->processCompounds(
				peakDetector->compounds, "compounds");
		peakDetector->processSlices(slices, "compounds");

		writeReport("compounds");
		delete_all(slices);
	}

	//process all mass slices
	if (peakDetector->processMassSlicesFlag == true) {
		peakDetector->matchRtFlag = false;
		peakDetector->checkConvergance = true;
		peakDetector->processMassSlices();
	}

	//cleanup
	delete_all(peakDetector->samples);
	peakDetector->samples.clear();
	peakDetector->allgroups.clear();

	return (0);
}

// vector<mzSlice*> getSrmSlices() {
// 	map<string, int> uniqSrms;
// 	vector<mzSlice*> slices;
// 	for (int i = 0; i < samples.size(); i++) {
// 		for (int j = 0; j < samples[i]->scans.size(); j++) {
// 			Scan* scan = samples[i]->scans[j];
// 			string filterLine = scan->filterLine;
// 			if (uniqSrms.count(filterLine) == 0) {
// 				uniqSrms[filterLine] = 1;
// 				slices.push_back(new mzSlice(filterLine));
// 			}
// 		}
// 	}
// 	cerr << "getSrmSlices() " << slices.size() << endl;
// 	return slices;
// }

void saveEICs(string filename) {
	ofstream myfile(filename.c_str());
	if (!myfile.is_open())
		return;
	for (int i = 0; i < peakDetector->allgroups.size(); i++) {
		PeakGroup& grp = peakDetector->allgroups[i];
		float rtmin = grp.minRt - 3;
		float rtmax = grp.maxRt + 3;

		myfile << "{\n";
		myfile << "\"groupId\": " << i << "," << endl;
		myfile << "\"rtmin\": " << rtmin << "," << endl;
		myfile << "\"rtmax\": " << rtmax << "," << endl;
		myfile << "\"eics\": [ " << endl;
		vector<EIC*> eics = getEICs(rtmin, rtmax, grp); //get EICs
		for (int j = 0; j < eics.size(); j++) {
			myfile << "{\n";
			saveEIC(myfile, eics[j]); //save EICs
			myfile << "}\n";
			if (j < eics.size() - 1)
				myfile << ",\n";
		}
		myfile << "]" << endl;
		myfile << "}" << endl;
		delete_all(eics); //cleanup
	}
	myfile.close();
}

void saveEIC(ofstream& out, EIC* eic) {
	int N = eic->rt.size();
	int count = 0;

	out << "\"label\":" << "\"" << eic->getSample()->sampleName << "\","
			<< endl;
	out << "\"data\": [";
	out << setprecision(4);
	for (int i = 0; i < N; i++) {
		if (eic->intensity[i] > 0) {
			if (count && i < N - 1)
				out << ",";
			out << "[" << eic->rt[i] << "," << eic->intensity[i] << "]";
			count++;
		};
	}
	out << "]\n";
}

vector<EIC*> getEICs(float rtmin, float rtmax, PeakGroup& grp) {
	vector<EIC*> eics;
	for (int i = 0; i < grp.peaks.size(); i++) {
		float mzmin = grp.meanMz - 0.2;
		float mzmax = grp.meanMz + 0.2;
		//cerr <<setprecision(5) << "getEICs: mz:" << mzmin << "-" << mzmax << " rt:" << rtmin << "-" << rtmax << endl;

		for (unsigned int j = 0; j < samples.size(); j++) {
			if (!grp.srmId.empty()) {
				EIC* eic = peakDetector->samples[j]->getEIC(grp.srmId);
				eics.push_back(eic);
			} else {
				EIC* eic = samples[j]->getEIC(mzmin, mzmax, rtmin, rtmax, 1);
				eics.push_back(eic);
			}
		}
	}
	return (eics);
}

void processOptions(int argc, char* argv[]) {

	//command line options
	const char * optv[] = { "a?alignSamples", "b?minGoodPeakCount <int>",
			"c?matchRtFlag", "d?db <sting>", "e?processMassSlicesFlag",
			"g:grouping_maxRtWindow <float>", "h?help",
			"i:minGroupIntensity <float>", "m?model <string>",
			"l?list  <string>", "o?outputdir <string>", "p?ppmMerge <float>",
			"r?rtStepSize <float>", "q:minQuality <float>", "v?ver",
			"w?minPeakWidth <int>", "y:?eicSmoothingWindow <int>",
			"z:minSignalBaseLineRatio <float>",
			NULL };

	//parse input options
	Options opts(*argv, optv);
	OptArgvIter iter(--argc, ++argv);
	const char * optarg;

	while (const char optchar = opts(iter, optarg)) {
		switch (optchar) {
		case 'a':
			peakDetector->alignSamplesFlag = true;
			break;

		case 'b':
			peakDetector->minGoodPeakCount = atoi(optarg);
			break;

		case 'c':
			peakDetector->matchRtFlag = true;
			break;

		case 'e':
			peakDetector->processMassSlicesFlag = true;
			break;

		case 'h':
			opts.usage(cerr, "files ...");
			exit(0);
			break;

		case 'i':
			peakDetector->minGroupIntensity = atof(optarg);
			break;

		case 'y':
			peakDetector->eic_smoothingWindow = atoi(optarg);
			break;

		case 'g':
			peakDetector->grouping_maxRtWindow = atof(optarg);
			break;

		case 'w':
			peakDetector->minNoNoiseObs = atoi(optarg);
			break;

		case 'r':
			peakDetector->rtStepSize = atoi(optarg);
			break;

		case 'p':
			peakDetector->ppmMerge = atof(optarg);
			break;

		case 'q':
			peakDetector->minQuality = atof(optarg);
			break;

		case 'z':
			peakDetector->minSignalBaseLineRatio = atof(optarg);
			break;

		case 'o':
			peakDetector->outputdir = optarg + string(DIR_SEPARATOR_STR);
			break;

		case 'd':
			peakDetector->ligandDbFilename = optarg;
			break;

		case 'm':
			clsfModelFilename = optarg;
			break;

		default:
			break;
		}
	}

	cerr << "#Command:\t";
	for (int i = 0; i < argc; i++)
		cerr << argv[i] << " ";
	cerr << endl;

	if (iter.index() < argc) {
		for (int i = iter.index(); i < argc; i++)
			filenames.push_back(argv[i]);
	}
}

void loadSamples(vector<string>&filenames) {
	cerr << "Loading samples" << endl;
	for (unsigned int i = 0; i < filenames.size(); i++) {
		cerr << "Loading " << filenames[i] << endl;
		mzSample* sample = new mzSample();
		sample->loadSample(filenames[i].c_str());
		sample->sampleName = cleanSampleName(filenames[i]);

		if (sample->scans.size() >= 1) {
			peakDetector->samples.push_back(sample);
			sample->summary();
		} else {
			if (sample != NULL) {
				delete sample;
				sample = NULL;
			}
		}
	}
	cerr << "loadSamples done: loaded " << peakDetector->samples.size()
			<< " samples\n";
}

string cleanSampleName(string sampleName) {
	unsigned int pos = sampleName.find_last_of("/");
	if (pos != std::string::npos) {
		sampleName = sampleName.substr(pos + 1, sampleName.length());
	}

	pos = sampleName.find_last_of("\\");
	if (pos != std::string::npos) {
		sampleName = sampleName.substr(pos + 1, sampleName.length());
	}
	return sampleName;
}

void loadCompoundCSVFile(string filename) {

	ifstream myfile(filename.c_str());
	if (!myfile.is_open())
		return;

	string line;
	string dbname = mzUtils::cleanFilename(filename);
	int loadCount = 0;
	int lineCount = 0;
	map<string, int> header;
	MassCalculator mcalc;

	while (getline(myfile, line)) {
		if (!line.empty() && line[0] == '#')
			continue;
		lineCount++;
		vector<string> fields;
		mzUtils::split(line, ',', fields);
		for (unsigned int i = 0; i < fields.size(); i++) {
			int n = fields[i].length();
			if (n > 2 && fields[i][0] == '"' && fields[i][n - 1] == '"') {
				fields[i] = fields[i].substr(1, n - 2);
			}
			if (n > 2 && fields[i][0] == '\'' && fields[i][n - 1] == '\'') {
				fields[i] = fields[i].substr(1, n - 2);
			}
		}

		if (lineCount == 1) {
			for (unsigned int i = 0; i < fields.size(); i++) {
				fields[i] = makeLowerCase(fields[i]);
				header[fields[i]] = i;
				//cerr << fields[i] << endl;
			}
			continue;
		}

		string id, name, formula;
		float rt = 0;
		float mz = 0;
		float charge = 0;
		float collisionenergy = 0;
		float precursormz = 0;
		float productmz = 0;
		int N = fields.size();
		if (header.count("mz") && header["mz"] < N)
			mz = string2float(fields[header["mz"]]);
		if (header.count("rt") && header["rt"] < N)
			rt = string2float(fields[header["rt"]]);
		if (header.count("expectedrt") && header["expectedrt"] < N)
			rt = string2float(fields[header["expectedrt"]]);
		if (header.count("charge") && header["charge"] < N)
			charge = string2float(fields[header["charge"]]);
		if (header.count("formula") && header["formala"] < N)
			formula = fields[header["formula"]];
		if (header.count("id") && header["id"] < N)
			id = fields[header["id"]];
		if (header.count("name") && header["name"] < N)
			name = fields[header["name"]];
		if (header.count("compound") && header["compound"] < N)
			name = fields[header["compound"]];
		if (header.count("precursormz") && header["precursormz"] < N)
			precursormz = string2float(fields[header["precursormz"]]);
		if (header.count("productmz") && header["productmz"] < N)
			productmz = string2float(fields[header["productmz"]]);
		if (header.count("collisionenergy") && header["collisionenergy"] < N)
			collisionenergy = string2float(fields[header["collisionenergy"]]);

		if (header.count("polarity") && header["polarity"] < N) {
			string x = fields[header["polarity"]];
			if (x == "+") {
				charge = 1;
			} else if (x == "-") {
				charge = -1;
			} else {
				charge = string2float(x);
			}

		}

		//cerr << "Loading: " << id << " " << formula << "mz=" << mz << " rt=" << rt << " charge=" << charge << endl;
		if (mz == 0)
			mz = precursormz;

		if (mz > 0 || !formula.empty()) {
			Compound* compound = new Compound(id, name, formula, charge);
			compound->expectedRt = rt;
			if (mz == 0)
				mz = mcalc.computeMass(formula, charge);
			compound->mass = mz;
			compound->db = dbname;
			compound->expectedRt = rt;
			compound->precursorMz = precursormz;
			compound->productMz = productmz;
			compound->collisionEnergy = collisionenergy;
			peakDetector->compounds.push_back(compound);
			loadCount++;
		}
	}
	sort(peakDetector->compounds.begin(), peakDetector->compounds.end(),
			Compound::compMass);
	cerr << "Loading " << dbname << " " << loadCount << endl;
	myfile.close();
}

//
// bool addPeakGroup(PeakGroup& grp1) {
//
// 	for (unsigned int i = 0; i < allgroups.size(); i++) {
// 		PeakGroup& grp2 = allgroups[i];
// 		float rtoverlap = mzUtils::checkOverlap(grp1.minRt, grp1.maxRt,
// 				grp2.minRt, grp2.maxRt);
// 		if (rtoverlap > 0.9 && ppmDist(grp1.meanMz, grp2.meanMz) < ppmMerge
// 				&& grp1.maxIntensity < grp2.maxIntensity) {
// 			return false;
// 		}
// 	}
//
// 	//cerr << "\t\t accepting " << grp1.meanMz << "@" << grp1.meanRt;
// 	allgroups.push_back(grp1);
// 	return true;
// }

void reduceGroups() {
	sort(allgroups.begin(), allgroups.end(), PeakGroup::compMz);
	cerr << "reduceGroups(): " << allgroups.size();
	for (unsigned int i = 0; i < allgroups.size(); i++) {
		PeakGroup& grp1 = allgroups[i];
		for (unsigned int j = i + 1; j < allgroups.size(); j++) {
			PeakGroup& grp2 = allgroups[j];

			float rtoverlap = mzUtils::checkOverlap(grp1.minRt, grp1.maxRt,
					grp2.minRt, grp2.maxRt);
			float ppmdist = ppmDist(grp2.meanMz, grp1.meanMz);
			if (ppmdist > ppmMerge)
				break;

			if (rtoverlap > 0.8 && ppmdist < ppmMerge) {
				if (grp1.maxIntensity <= grp2.maxIntensity) {
					allgroups.erase(allgroups.begin() + i);
					i--;
					break;
				} else if (grp1.maxIntensity > grp2.maxIntensity) {
					allgroups.erase(allgroups.begin() + j);
					i--;
					break;
				}
			}
		}
	}
	cerr << " done final group count(): " << allgroups.size() << endl;
}

void writeReport(string setName) {

	if (peakDetector->writeCSVFlag == false)
		return;

	//create an output folder
	mzUtils::createDir(outputdir.c_str());

	cerr << "writeReport() " << allgroups.size() << " groups ";
//	if (reduceGroupsFlag)
//		reduceGroups();
	saveEICs(outputdir + setName + ".eics.json");
	writePeakTableXML(outputdir + setName + ".mzroll");
	writeCSVReport(outputdir + setName + ".csv");
}

void writeCSVReport(string filename) {
	ofstream groupReport;
	groupReport.open(filename.c_str());
	if (!groupReport.is_open())
		return;

	groupReport
			<< "label,metaGroupId,groupId,goodPeakCount,medMz,medRt,maxQuality,note,compound,compoundId,expectedRtDiff,ppmDiff,parent";
	for (unsigned int i = 0; i < samples.size(); i++) {
		groupReport << "," << samples[i]->sampleName;
	}
	groupReport << endl;

	int groupId = 0;

	for (int i = 0; i < allgroups.size(); i++) {
		PeakGroup* group = &allgroups[i];
		groupId = i;

		vector<float> yvalues = group->getOrderedIntensityVector(samples,
				PeakGroup::AreaTop);    //areatop
		//if ( group->metaGroupId == 0 ) { group->metaGroupId=groupId; }

		string tagString = group->srmId + group->tagString;
		tagString = mzUtils::substituteInQuotedString(tagString, "\",'", "---");
		char label[2];
		sprintf(label, "%c", group->label);

		groupReport << label << "," << setprecision(7) << group->metaGroupId
				<< "," << groupId << "," << group->goodPeakCount << ", "
				<< group->meanMz << "," << group->meanRt << ","
				<< group->maxQuality << "," << tagString;

		string compoundName;
		string compoundID;
		float expectedRtDiff = 0;
		float ppmDist = 0;

		if (group->compound != NULL) {
			compoundName = mzUtils::substituteInQuotedString(
					group->compound->name, "\",'", "---");
			compoundID = group->compound->id;
			ppmDist = mzUtils::ppmDist((double) group->compound->mass,
					(double) group->meanMz);
			expectedRtDiff = group->expectedRtDiff;
		}

		groupReport << "," << compoundName;
		groupReport << "," << compoundID;
		groupReport << "," << expectedRtDiff;
		groupReport << "," << ppmDist;

		if (group->parent != NULL) {
			groupReport << "," << group->parent->meanMz;
		} else {
			groupReport << "," << group->meanMz;
		}

		for (unsigned int j = 0; j < samples.size(); j++)
			groupReport << "," << yvalues[j];
		groupReport << endl;
	}
}

void writeSampleListXML(xml_node& parent) {
	xml_node samplesset = parent.append_child();
	samplesset.set_name("samples");

	for (int i = 0; i < samples.size(); i++) {
		xml_node _sample = samplesset.append_child();
		_sample.set_name("sample");
		_sample.append_attribute("name") = samples[i]->sampleName.c_str();
		_sample.append_attribute("filename") = samples[i]->fileName.c_str();
		_sample.append_attribute("sampleOrder") = i;
		_sample.append_attribute("setName") = "A";
		_sample.append_attribute("sampleName") = samples[i]->sampleName.c_str();
	}
}

void writePeakTableXML(std::string filename) {

	xml_document doc;
	doc.append_child().set_name("project");
	xml_node peak = doc.child("project");
	peak.append_attribute("Program") = "peakdetector";
	peak.append_attribute("Version") = "Mar112012";

	writeSampleListXML(peak);
	writeParametersXML(peak);

	if (peakDetector->allgroups.size()) {
		peak.append_child().set_name("PeakGroups");
		xml_node peakgroups = peak.child("PeakGroups");
		for (int i = 0; i < peakDetector->allgroups.size(); i++) {
			writeGroupXML(peakgroups, &peakDetector->allgroups[i]);
		}
	}

	doc.save_file(filename.c_str());
}

void writeParametersXML(xml_node& parent) {

	xml_node p = parent.append_child();
	p.set_name("PeakDetectionParameters");
	p.append_attribute("alignSamples") = peakDetector->alignSamplesFlag;
	p.append_attribute("matchRt") = peakDetector->matchRtFlag;
	p.append_attribute("ligandDbFilename") =
			peakDetector->ligandDbFilename.c_str();
	p.append_attribute("clsfModelFilename") = clsfModelFilename.c_str();
	p.append_attribute("rtStepSize") = peakDetector->rtStepSize;
	p.append_attribute("ppmMerge") = peakDetector->ppmMerge;
	p.append_attribute("eic_smoothingWindow") =
			peakDetector->eic_smoothingWindow;
	p.append_attribute("grouping_maxRtWindow") =
			peakDetector->grouping_maxRtWindow;
	p.append_attribute("minGoodPeakCount") = peakDetector->minGoodPeakCount;
	p.append_attribute("minSignalBlankRatio") =
			peakDetector->minSignalBlankRatio;
	p.append_attribute("minPeakWidth") = peakDetector->minNoNoiseObs;
	p.append_attribute("minSignalBaseLineRatio") =
			peakDetector->minSignalBaseLineRatio;
	p.append_attribute("minGroupIntensity") = peakDetector->minGroupIntensity;
	p.append_attribute("minQuality") = peakDetector->minQuality;
}

void writeGroupXML(xml_node& parent, PeakGroup* g) {
	if (!g)
		return;

	xml_node group = parent.append_child();
	group.set_name("PeakGroup");

	group.append_attribute("groupId") = g->groupId;
	group.append_attribute("tagString") = g->tagString.c_str();
	group.append_attribute("metaGroupId") = g->metaGroupId;
	group.append_attribute("expectedRtDiff") = g->expectedRtDiff;
	group.append_attribute("groupRank") = (float) g->groupRank;
	group.append_attribute("label") = g->label;
	group.append_attribute("type") = (int) g->type();
	group.append_attribute("changeFoldRatio") = (int) g->changeFoldRatio;
	group.append_attribute("changePValue") = (int) g->changePValue;
	if (g->srmId.length())
		group.append_attribute("srmId") = g->srmId.c_str();

	if (g->hasCompoundLink()) {
		Compound* c = g->compound;
		group.append_attribute("compoundId") = c->id.c_str();
		group.append_attribute("compoundDB") = c->db.c_str();
	}

	for (int j = 0; j < g->peaks.size(); j++) {
		Peak& p = g->peaks[j];
		xml_node peak = group.append_child();
		peak.set_name("Peak");
		peak.append_attribute("pos") = ((int) p.pos);
		peak.append_attribute("minpos") = (int) p.minpos;
		peak.append_attribute("maxpos") = (int) p.maxpos;
		peak.append_attribute("rt") = p.rt;
		peak.append_attribute("rtmin") = p.rtmin;
		peak.append_attribute("rtmax") = p.rtmax;
		peak.append_attribute("mzmin") = p.mzmin;
		peak.append_attribute("mzmax") = p.mzmax;
		peak.append_attribute("scan") = (int) p.scan;
		peak.append_attribute("minscan") = (int) p.minscan;
		peak.append_attribute("maxscan") = (int) p.maxscan;
		peak.append_attribute("peakArea") = p.peakArea;
		peak.append_attribute("peakAreaCorrected") = p.peakAreaCorrected;
		peak.append_attribute("peakAreaTop") = p.peakAreaTop;
		peak.append_attribute("peakAreaFractional") = p.peakAreaFractional;
		peak.append_attribute("peakRank") = p.peakRank;
		peak.append_attribute("peakIntensity") = p.peakIntensity;
		;
		peak.append_attribute("peakBaseLineLevel") = p.peakBaseLineLevel;
		peak.append_attribute("peakMz") = p.peakMz;
		peak.append_attribute("medianMz") = p.medianMz;
		peak.append_attribute("baseMz") = p.baseMz;
		peak.append_attribute("quality") = p.quality;
		peak.append_attribute("width") = (int) p.width;
		peak.append_attribute("gaussFitSigma") = p.gaussFitSigma;
		peak.append_attribute("gaussFitR2") = p.gaussFitR2;
		peak.append_attribute("groupNum") = p.groupNum;
		peak.append_attribute("noNoiseObs") = (int) p.noNoiseObs;
		peak.append_attribute("noNoiseFraction") = p.noNoiseFraction;
		peak.append_attribute("symmetry") = p.symmetry;
		peak.append_attribute("signalBaselineRatio") = p.signalBaselineRatio;
		peak.append_attribute("groupOverlap") = p.groupOverlap;
		peak.append_attribute("groupOverlapFrac") = p.groupOverlapFrac;
		peak.append_attribute("localMaxFlag") = p.localMaxFlag;
		peak.append_attribute("fromBlankSample") = p.fromBlankSample;
		peak.append_attribute("label") = p.label;
		peak.append_attribute("sample") = p.getSample()->sampleName.c_str();
	}

	if (g->childCount()) {
		for (int i = 0; i < g->children.size(); i++) {
			PeakGroup* child = &(g->children[i]);
			writeGroupXML(group, child);
		}
	}
}