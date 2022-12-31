  // Copyright 2022 Mironov Nikita
#include "../../../modules/task_3/mironov_n_grahams_passage/grahams_passage.h"

Point::Point() {
  x = 0;
  y = 0;
}

Point::Point(double _x, double _y) : x{ _x }, y{ _y } {
}

Point Point::Min(const Point& firstPoint, const Point& secondPoint) {
  if (firstPoint.y < secondPoint.y) {
    return firstPoint;
  } else {
    if ((firstPoint.y == secondPoint.y) && (firstPoint.x < secondPoint.x))
      return firstPoint;
  }
  return secondPoint;
}

Point::~Point() {
}

Vector::Vector() {
  this -> X = 0;
  this -> Y = 0;
}

Vector::Vector(Point firstPoint, Point secondPoint) {
  this -> X = secondPoint.x - firstPoint.x;
  this -> Y = secondPoint.y - firstPoint.y;
}

Vector::Vector(double x, double y) {
  this -> X = x;
  this -> Y = y;
}

double Modul(const Vector& vector) {
  return (sqrt(vector.X * vector.X + vector.Y * vector.Y));
}

double Scalar(const Vector& vectorFirst, const Vector& vectorSecond) {
  return (vectorFirst.X * vectorSecond.X + vectorFirst.Y * vectorSecond.Y);
}

Vector::~Vector() {
}


double Cosinus(const Vector& vectorFirst, const Vector& vectorSecond) {
  return (Scalar(vectorFirst, vectorSecond) /
          (Modul(vectorFirst) * Modul(vectorSecond)));
}

Point SearchMinPoint(const std::vector<Point>& Points) {
  Point Minimum = Points[0];
  for (size_t i = 1; i < Points.size(); i++) {
    Minimum = Minimum.Min(Minimum, Points[i]);
  }
  return Minimum;
}

std::vector<Point> GeneratePoints(int pointsCount) {
  if (pointsCount < 1)
    throw "Points count error";
  std::vector<Point> P(pointsCount);
  std::mt19937 generator;
  for (int i = 0; i < pointsCount; i++) {
    P[i].x = generator();
    P[i].y = generator();
  }
  return P;
}

std::vector<Point> SortPoints(const std::vector<Point>& Points) {
  std::vector<Point> P = Points;
  Point Min = SearchMinPoint(P);
  std::vector<double> Cos(P.size());
  Vector VecA(1, 0);
  for (size_t i = 0; i < P.size(); i++) {
    Vector VecB(Min, Points[i]);
    if (Min == Points[i]) {
      Cos[i] = 2;
    } else {
      Cos[i] = Cosinus(VecA, VecB);
    }
  }
  for (size_t i = 0; i < P.size() - 1; i++) {
    for (size_t j = 0; j < P.size() - i - 1; j++) {
      if (Cos[j + 1] > Cos[j]) {
        Point tmp = P[j + 1];
        P[j + 1] = P[j];
        P[j] = tmp;
        double tmpCos = Cos[j + 1];
        Cos[j + 1] = Cos[j];
        Cos[j] = tmpCos;
      }
    }
  }
  for (size_t i = 0; i < P.size() - 1; i++) {
    if (fabs(Cos[i + 1] - Cos[i]) < 0.000001) {
      Vector Vec1(Min, P[i]);
      Vector Vec2(Min, P[i + 1]);
      if (Modul(Vec1) > Modul(Vec2)) {
        Point tmp = P[i + 1];
        P[i + 1] = P[i];
        P[i] = tmp;
      }
    }
  }
  return P;
}

bool sideDef(double x1, double y1, double x2, double y2, double x3, double y3) {
  return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1) < 0;
}

bool pointsEqual(double x1, double y1, double x2, double y2) {
  return (fabs(x1 - x2) < 0.000001) && (fabs(y1 - y2) < 0.000001);
}

std::vector<Point> GrahamsPasOneProc(const std::vector<Point>& P) {
  std::vector<Point> Points = SortPoints(P);
  if (Points.size() < 2) {
    return Points;
  }
  std::vector<double> Result;
  Result.push_back(Points[0].x);
  Result.push_back(Points[0].y);
  Result.push_back(Points[1].x);
  Result.push_back(Points[1].y);
  if (Points.size() > 2) {
    for (size_t i = 2; i < Points.size(); i++) {
      while (
        sideDef(
        Result[Result.size() - 4],
        Result[Result.size() - 3],
        Result[Result.size() - 2],
        Result[Result.size() - 1],
        Points[i].x, Points[i].y)) {
        Result.pop_back();
        Result.pop_back();
      }
      Result.push_back(Points[i].x);
      Result.push_back(Points[i].y);
    }
  }
  std::vector<Point> resultPoint(Result.size() / 2);
  for (size_t i = 0; i < Result.size(); i += 2) {
    resultPoint[i / 2].x = Result[i];
    resultPoint[i / 2].y = Result[i + 1];
  }
  return resultPoint;
}

std::vector<Point> GrahamsPasParal(const std::vector<Point>& Points) {
  std::vector<Point> P = SortPoints(Points);
  int pointsCount = P.size();
  MPI_Status status;
  int procId, procCount;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);
  MPI_Comm_rank(MPI_COMM_WORLD, &procId);
  int pointsCountL = pointsCount / procCount;
  if (pointsCount >= procCount) {
    std::vector<double> Vec;
    for (size_t i = 0; i < P.size(); i++) {
      Vec.push_back(P[i].x);
      Vec.push_back(P[i].y);
    }
    std::vector<double> vecL(2 * pointsCountL);
    MPI_Scatter(&Vec[0], 2 * pointsCountL, MPI_DOUBLE, &vecL[0],
                2 * pointsCountL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (procId == (procCount - 1)) {
      for (size_t i = procCount * pointsCountL; i < P.size(); i++) {
        vecL.push_back(P[i].x);
        vecL.push_back(P[i].y);
      }
    }
    std::vector<Point> pL(vecL.size() / 2);
    for (size_t i = 0; i < vecL.size(); i += 2) {
      pL[i / 2].x = vecL[i];
      pL[i / 2].y = vecL[i + 1];
    }
    std::vector<Point> l_resPoint = GrahamsPasOneProc(pL);
    vecL.clear();
    for (size_t i = 0; i < l_resPoint.size(); i++) {
      vecL.push_back(l_resPoint[i].x);
      vecL.push_back(l_resPoint[i].y);
    }
    int vecN = vecL.size();
    std::vector<int> sizeVector(procCount);
    int N = 0;
    MPI_Gather(&vecN, 1, MPI_INT, &sizeVector[0], 1,
    MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(&vecN, &N, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    std::vector<double> globalVector(N);
    if (procId == 0) {
      std::vector<int> addr(procCount);
      addr[0] = 0;
      for (int i = 1; i < procCount; i++) {
        addr[i] = sizeVector[i - 1];
      }
      for (int i = 2; i < procCount; i++) {
        addr[i] += addr[i - 1];
      }
      for (int i = 0; i < vecN; i++) {
        globalVector[i] = vecL[i];
      }
      for (int i = 1; i < procCount; i++) {
        MPI_Recv(&globalVector[addr[i]], sizeVector[i],
                MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      }
    } else {
      MPI_Send(&vecL[0], vecN, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    if (procId == 0) {
      std::vector<Point> globalResPoint(globalVector.size() / 2);
      for (size_t i = 0; i < globalVector.size(); i += 2) {
        globalResPoint[i / 2].x = globalVector[i];
        globalResPoint[i / 2].y = globalVector[i + 1];
      }
      std::vector<Point> resPoint = GrahamsPasOneProc(globalResPoint);
      return resPoint;
    }
  } else {
    std::vector<Point> resPoint = GrahamsPasOneProc(P);
    return resPoint;
  }
  return P;
}
