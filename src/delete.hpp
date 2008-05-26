#ifndef DELETE_HPP_INCLUDED
#define DELETE_HPP_INCLUDED



/**
 *

#define ABS(x) (((x) > 0) ? (x) : (-(x)))
bool removePoint(PMF *Hist, PMF *newHist, int PointThre, int zap, struct borderPoint * Bord)
{
  float kg, kd, kat, nkat, len, zmX, zmY, cosinus;
  long oldSize, idPktu, zm, i1, i2, id1, id2;
  struct Tpoint *pkt, *nowy, *akt, *tmp;
  long los, k, births;
  int changed;
  bool restore;

  Hist->setPointIDs();		// To jest BARDZO wazne tutaj !!!

  FILE *fstep;
  if(zap == 1) {
      fstep = fopen("STEPSSeed1Iter981.txt", "w");
      writeConf(fstep, Hist);
      fflush(fstep);
  }

  oldSize = Hist->getSize();
  if(oldSize == 0) return(restore);
  births = Hist->countBirths();
  idPktu = oldSize;
  los = rand() % births + 1;
  //los = 1;
  k = 0;
  while( k < los  &&  (pkt = Hist->get()) != NULL )
  {
    if(pkt->r1 != NULL  &&  pkt->r2 != NULL
	&&  pkt->x < pkt->r1->x  &&  pkt->x < pkt->r2->x)
    {
      k++;
      if(k == los) break;
    }
    newHist->addPoint(pkt);
  }

  listB *qB = new listB();	// Lista punktow wylosowanych
  listI *qI = new listI();	// Lista punktow przeciecia
  while(!Hist->empty())
  {
    qB->put( Hist->get() );
  }

  struct Tpoint *tmp1, *tmp2;
  if (zap == 1)
  {
      fprintf(fstep, "\nUSUWAM PKT : %3li ", pkt->id);
      fprintf(fstep, "[%.3f;%.3f;%li;%li]\n", pkt->x, pkt->y,
	      (pkt->r1) ? pkt->r1->id : 0,
	      (pkt->r2) ? pkt->r2->id : 0);
      qB->write(fstep);
      qI->write(fstep);
      fflush(fstep);
  }
  los = pkt->id - 1;
  tmp = allocPoint(pkt->x, pkt->y, pkt, NULL, 0, 0, &los);

  id1 = pkt->r1->id;
  delPathS(tmp, id1, qB, qI, &idPktu, Bord);

  tmp->r1 = NULL;
  tmp->r2 = pkt;
  id2 = pkt->r2->id;
  delPathS(tmp, id2, qB, qI, &idPktu, Bord);
  delete(pkt);
  delete(tmp);

  long qq = 0;
  restore = false;
  changed = 0;
  while( !qB->empty() || !qI->empty() )
  {
    akt = doGet( qB, qI, &id1, &id2 );
    newHist->addPoint(akt);
    if( akt->id > oldSize )
	if( (++changed) > PointThre )
	{
	    restore = true;
	    break;
	}

    if( akt->id <= oldSize )
    {
      if(akt->r1 != NULL  &&  akt->r2 == NULL  &&  isInsideBorder(akt->x,akt->y,Bord) == 0
	  &&  isOnBorder(akt->x,akt->y,Bord) < 0
	)
      {
	if(ABS(akt->x - akt->r1->x) < EPSILON) {
	  // fprintf(stderr, "\n [OLD] UPSY DAIZY!\n");
	  if(akt->y > akt->r1->y)  kat = 0.5*M_PI;
	  else kat = -0.5*M_PI;
	}
	else
	  kat = atan((akt->y - akt->r1->y) / (akt->x - akt->r1->x));
	determineUpdateAngle(&nkat);
	nkat += kat;
	if(nkat > M_PI/2.0) nkat -= M_PI;
	if(nkat < -M_PI/2.0) nkat += M_PI;

	len = E(2);
	zmX = akt->x + len*cos(nkat);
	zmY = akt->y + len*sin(nkat);
	nowy = allocPoint(zmX, zmY, akt, NULL, 0, 0, &idPktu);
	storePoints2(nowy, qB, qI, &idPktu, Bord);
	akt->r2 = nowy;
      }
    }
    else {
      if(akt->r1 != NULL  &&  akt->r2 == NULL  &&  isInsideBorder(akt->x,akt->y,Bord) == 0
	  &&  isOnBorder(akt->x,akt->y,Bord) < 0
	)
      {
	if(ABS(akt->x - akt->r1->x) < EPSILON) {
	  if(akt->y > akt->r1->y)  kat = 0.5*M_PI;
	  else kat = -0.5*M_PI;
	}
	else
	  kat = atan((akt->y - akt->r1->y) / (akt->x - akt->r1->x));
	determineUpdateAngle(&nkat);
	nkat += kat;
	if(nkat > M_PI/2.0) nkat -= M_PI;
	if(nkat < -M_PI/2.0) nkat+= M_PI;

	len = E(2);
	zmX = akt->x + len*cos(nkat);
	zmY = akt->y + len*sin(nkat);
	nowy = allocPoint(zmX, zmY, akt, NULL, 0, 0, &idPktu);
	storePoints2(nowy, qB, qI, &idPktu, Bord);
	akt->r2 = nowy;
      }
      if(akt->r1 != NULL  &&  akt->r2 != NULL
	  &&  akt->r1->x < akt->x  &&  akt->r2->x < akt->x)
      {
	// isIDaNeighbor
	int i1, i2;
	if( (i1 = isIDaNeighbor(akt->r1, id1)) > 0
	    &&  (i2 = isIDaNeighbor(akt->r2, id2)) > 0 )
	{
	  if(i1 == 1)  akt->r1->r1 = akt;
	  else if(i1 == 2)  akt->r1->r2 = akt;
	  else fprintf(stderr, "&1");

	  if(i2 == 1)  akt->r2->r1 = akt;
	  else if(i2 == 2)  akt->r2->r2 = akt;
	  else fprintf(stderr, "&2");
	}
	else
	if( (i1 = isIDaNeighbor(akt->r1, id2)) > 0
	    &&  (i2 = isIDaNeighbor(akt->r2, id1)) > 0)
	{
	  if(i1 == 1)  akt->r1->r1 = akt;
	  else if(i1 == 2)  akt->r1->r2 = akt;
	  else fprintf(stderr, "&1");

	  if(i2 == 1)  akt->r2->r1 = akt;
	  else if(i2 == 2)  akt->r2->r2 = akt;
	  else fprintf(stderr, "&2");
	}
	else
	  fprintf(stderr, "#");

	delPathS(akt, id1, qB, qI, &idPktu, Bord);
	delPathS(akt, id2, qB, qI, &idPktu, Bord);
      }
    }
    qB->remove(akt->id);
    qI->remove(akt->id);

    if(zap == 1) {
	fprintf(fstep, "\n\n\nKONFIGURACJA w kroku %li\n\n", qq);
	writeConf(fstep, newHist);
	qB->write(fstep);
	qI->write(fstep);
	fflush(fstep);
    }

    qq++;
    checkPoint(akt, qq);
    detectSegment(akt, qq);
  }
  qB->free();  qI->free();
  delete(qB);  delete(qI);

  if(zap == 1) {
      writeConf(fstep, Hist);
      fclose(fstep);
      fflush(fstep);
  }


  fprintf(stderr, "?");  fflush(stderr);
  struct T_PMFElement *ptr = newHist->giveHead();
  while(ptr)
  {
    checkPoint(ptr->point, 0);
    detectSegment(ptr->point, 0);
    ptr = ptr->next;
  }

  return(restore);
} // end function removePoint
#undef ABS
 */

#endif // DELETE_HPP_INCLUDED
