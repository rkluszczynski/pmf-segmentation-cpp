
#include "grayscaleimage.hpp"


void SimulateBinarySegmentation (
                            char * imageFileName,
                            PMF<double> * pmf,
                            long iterations,
                            double PMFRate
                        )
{
    //* Initializing some variables. *
    GrayScaleImage gsimg(imageFileName);


    double    beta_1 = 20.0;
    double    beta_2 = 0.0;
    double areaOfPMF = M_PI * pmf->GetPMFHeight() * pmf->GetPMFWidth();

    double tmpArea, tmpELen, storedArea, storedELen;
    tmpArea = storedArea = gsimg.CalculateScanLinesEnergy(pmf);
    tmpELen = storedELen = 0.0; //estimateEdgesLen(Hist);

    double engH = beta_1 * storedArea + beta_2 * storedELen;


    //* Main loop of brilliant, image segmentation algorithm:-) *
    long loopIteration = 1;				//* Iteration counter *
    bool runSimulation = false;
    if(iterations == 0  &&  PMFRate == 0.0)  runSimulation = true;
                                          //* ----------------- *
                                          //*  Simulation loop  *
                                          //* ----------------- *
    while(runSimulation)
    {
        // * Setting inverse temperature. *
        beta_1 = 20.0 + 0.009 * loopIteration; //powf((float)loopIteartion, 1.0);
        beta_2 = 0.0;

        // * Recalculate energy for new values of inverse temperature. *
        engH = beta_1 * storedArea + beta_2 * storedELen;


        /* Random choice of the operation for PMF modification. *
        size = (float)Hist->getSize();
        noOfBirths = (float)Hist->countBirths();
        limit = noOfBirths / (areaOfPMF + noOfBirths);
        los = (float)rand() / RAND_MAX;

        cpConf(Hist, Htmp);
        if(los <= limit)  { removePoint(Hist,H2); }
        else {
            x = WIDTH  * rand() / RAND_MAX;
            y = HEIGHT * rand() / RAND_MAX;
            addPoint(Hist, H2, x, y);
        }

        //* Calculating energy of modified configuration. *
        H2->setPointIDs();
        tmpArea = monte2(H2, orientation);
        tmpELen = estimateEdgesLen(H2);
        newH = beta_1 * tmpArea + beta_2 * tmpELen;

        //* Mechanism of simulated annealing. *
        if(newH <= engH)
        {
            swap = Hist;  Hist = H2;  H2 = swap;
            storedArea = tmpArea;
            storedELen = tmpELen;
            //* Showing size and PMF rate of better configuration. *
            fprintf(stderr, "(%.0f-%1.4f)", size, storedArea);
        }
        else {
            limit = exp(- (newH - engH));
            los = (float)rand() / RAND_MAX;
            if(los < limit)
            {
                swap = Hist;  Hist = H2;  H2 = swap;
                storedArea = tmpArea;
                storedELen = tmpELen;
            }
            else {
                Hist->freeMem();
                cpConf(Htmp, Hist);
            }
        }
        H2->freeMem();
        Htmp->freeMem();

        //* Saving value of PMF rate. *
        if(graphStep != 0)
        {
            if(qq % graphStep == 0)  fprintf(PMRfd, "%f\n", storedArea);
            fflush(PMRfd);
        }
        */
        //* Testing conditions for stoping simulation. *
        fprintf(stderr, ".");  fflush(stderr);
        loopIteration++;

        if (iterations > 0  &&  loopIteration >= iterations)  runSimulation = false;
        if (PMFRate > 0.0  &&  PMFRate > storedArea) runSimulation = false;
    }
    //* ------------------------- *
    //*  End of simulation loop.  *
    //* ------------------------- *

    return;
}


/*

//* ********************************************************************** *
//*                                                                        *
//*                              MAIN BLOCK                                *
//*                                                                        *
//* ********************************************************************** *

int simulatePMF ( char *pictureFile,
		  char *configurationFile,
		  char *outputFile,
		  long iterations,
		  float PMFRate,
		  float fieldSize,
		  long graphStep
		)
{
  qTab *Hist, *H2, *HR, *swap, *Htmp;
  time_t tmp, timeBefore, timeAfter;
  long zm;
  float x, y;
  float areaOfPMF, size, noOfBirths, los, limit, engH, newH;
  float beta_1, beta_2;
  int orientation = 1;
  float storedArea,storedELen,tmpArea,tmpELen,oldArea;
  FILE *PMRfd, *stdoutStream = fdopen(dup(fileno(stdout)), "w");


  tmp = ZARODEK;  srand(tmp);
  HEIGHT = fieldSize;
  WIDTH  = fieldSize;

  //* Loading picture data. *
  readPicture(pictureFile);

  //* Setting initial configuration. *
  fprintf(stderr, "[INIT] Setting initial configuration ... ");  fflush(stderr);
  Hist = new qTab();
  if(strcmp(configurationFile, "") != 0)
  {
    if( loadFromFile(configurationFile, Hist) < 0 ) {
      fprintf(stderr, "\n[ERROR] Still can't load configuration from file '%s'\n", configurationFile);
      exit(2);
    }
  }
  else { GeneratePMF(Hist); }
  fprintf(stderr, "done.\n");  fflush(stderr);
  Hist->setPointIDs();

  //* Initializing some variables. *
  H2 = new qTab();
  Htmp = new qTab();
  timeBefore = time(NULL);

  beta_1 = 20.0;
  beta_2 = 0.0;
  areaOfPMF = M_PI * HEIGHT * WIDTH;

  tmpArea = storedArea = monte2(Hist,orientation);
  tmpELen = storedELen = estimateEdgesLen(Hist);

  engH = beta_1 * storedArea + beta_2 * storedELen;

  //* Opening file for saving misclasification rate. *
  if(graphStep != 0)
  {
    char PMRFile[256];
    sprintf(PMRFile, "%s.PMR", outputFile);

    if((PMRfd = fopen(PMRFile, "w")) != NULL)
    {
      fprintf(stderr, "[INFO] Saving misclasification rates every %li steps to file '%s'.\n",
	graphStep, PMRFile);
      fprintf(PMRfd, "Saving every %li step(s).\n\n", graphStep);
      fprintf(PMRfd, "%f\n", storedArea);
      fflush(PMRfd);
    }
    else {
	   fprintf(stderr, "[WARNING] Can't open file '%s' for writing.", PMRFile);
	   fprintf(stderr, "[WARNING] Won't save mislasification rates ...");
	   graphStep = 0;
	 }
  }

  //* Main loop of brilliant, image segmentation algorithm:-) *
  long qq = 1;				//* Iteration counter *
  int stopSimulation = 0;
  if(iterations == 0  &&  PMFRate == 0.0)  stopSimulation = 1;
                                        //* ----------------- *
                                        //*  Simulation loop  *
                                        //* ----------------- *
  while(stopSimulation == 0)
  {
    //* Reversing direction of evolution. *
    if((qq % 25 == 0) || (qq > 1 && qq < 49))
    {
	HR = new qTab();
        orientation *= -1;
        reverse(Hist,HR);
	cpConf(HR, Htmp);
	HR->freeMem();
	delete(HR);
	Hist->freeTab();	// Bo punkty zwolnilismy w HR !!
	cpConf(Htmp, Hist);
	Htmp->freeMem();
        Hist->setPointIDs();
    };

    //* Setting inverse temperature. *
    beta_1 = 20.0 + 0.009 * qq; //powf((float)qq,1.0);
    beta_2 = 0.0;

    //* Recalculate energy for new values of inverse temperature. *
    engH = beta_1 * storedArea + beta_2 * storedELen;

    //* Random choice of the operation for PMF modification. *
    size = (float)Hist->getSize();
    noOfBirths = (float)Hist->countBirths();
    limit = noOfBirths / (areaOfPMF + noOfBirths);
    los = (float)rand() / RAND_MAX;

    cpConf(Hist, Htmp);
    if(los <= limit)  { removePoint(Hist,H2); }
    else {
      x = WIDTH  * rand() / RAND_MAX;
      y = HEIGHT * rand() / RAND_MAX;
      addPoint(Hist, H2, x, y);
    }

    //* Calculating energy of modified configuration. *
    H2->setPointIDs();
    tmpArea = monte2(H2, orientation);
    tmpELen = estimateEdgesLen(H2);
    newH = beta_1 * tmpArea + beta_2 * tmpELen;

    //* Mechanism of simulated annealing. *
    if(newH <= engH)
    {
      swap = Hist;  Hist = H2;  H2 = swap;
      storedArea = tmpArea;
      storedELen = tmpELen;
      //* Showing size and PMF rate of better configuration. *
      fprintf(stderr, "(%.0f-%1.4f)", size, storedArea);
    }
    else {
      limit = exp(- (newH - engH));
      los = (float)rand() / RAND_MAX;
      if(los < limit)
      {
        swap = Hist;  Hist = H2;  H2 = swap;
        storedArea = tmpArea;
        storedELen = tmpELen;
      }
      else {
        Hist->freeMem();
        cpConf(Htmp, Hist);
      }
    }
    H2->freeMem();
    Htmp->freeMem();

    //* Saving value of PMF rate. *
    if(graphStep != 0)
    {
       if(qq % graphStep == 0)  fprintf(PMRfd, "%f\n", storedArea);
       fflush(PMRfd);
    }

    //* Testing conditions for stoping simulation. *
    fprintf(stderr, ".");  fflush(stderr);
    qq++;
    if(iterations != 0)
    {  if(qq >= iterations) stopSimulation = 1;  }
    if(PMFRate != 0.0)
    {  if(PMFRate > storedArea) stopSimulation = 1;  }
  }
  //* ------------------------- *
  //*  End of simulation loop.  *
  //* ------------------------- *

  if(graphStep != 0)  fclose(PMRfd);

  //* Changing the orientation, if it was 'reversed'. *
  if(orientation == -1)
  {
     reverse(Hist, H2);
     Htmp = Hist;
     Hist = H2;
     H2 = Htmp;
  }
  delete(H2);
  delete(Htmp);

  //* Printing the summary of the simulation. *
  timeAfter = time(NULL);
  if(strcmp(outputFile, "") != 0)
  {
    freopen(outputFile, "w+", stdoutStream);
    fprintConf(stdoutStream, Hist);
    fprintf(stdoutStream, "\n Calculation time : %limin. %lisec.\n",
                     (timeAfter-timeBefore)/60, (timeAfter-timeBefore)%60);
    fprintf(stdoutStream, " Picture file : %s\n", pictureFile);
    fprintf(stdoutStream, " Misclassification rate = %1.4f\n", storedArea);
    fprintf(stdoutStream, " Iterations = %li\n", qq);
    fprintf(stdoutStream, " Average iteration time : %fsec.\n",
                              ((float)(timeAfter-timeBefore))/((float)qq));
    fprintf(stdoutStream, " Seed = %li\n", tmp);
    fflush(stdoutStream);
    fclose(stdoutStream);
  }
  else  { fprintConf(stdoutStream, Hist); }
   fprintf(stderr, "\n[DONE]        Calculation time : %limin. %lisec.\n",
                     (timeAfter-timeBefore)/60, (timeAfter-timeBefore)%60);
   fprintf(stderr, "[DONE]            Picture file : %s\n", pictureFile);
   fprintf(stderr, "[DONE]  Misclassification rate = %1.4f\n", storedArea);
   fprintf(stderr, "[DONE]              Iterations = %li\n", qq);
   fprintf(stderr, "[DONE]  Average iteration time : %fsec.\n",
                              ((float)(timeAfter-timeBefore))/((float)qq));
   fprintf(stderr, "[DONE]                    Seed = %li\n", tmp);

  Hist->freeMem();
  delete(Hist);
//  saveSeed("seed.dat", tmp);

  return(0);
}
//*/
