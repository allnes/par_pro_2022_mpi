// Copyright 2022 Bushmakin Maxim
#include "../../../modules/task_3/bushmakin_m_mark_binary/mark_binary.h"

MarkImage::MarkImage(int _w, int _h) {
    w = _w;
    h = _h;
    elements.reserve(w * h);
    for (int i = 0; i < w * h; i++)
        elements.push_back(0);
}

MarkImage::MarkImage(const MarkImage &img) : elements(img.elements) {
    w = img.w;
    h = img.h;
}

int MarkImage::W() const { return w; }

int MarkImage::H() const { return h; }

void MarkImage::Random() {
    std::random_device dev;
    std::mt19937 gen(dev());

    for (int i = 0; i < w * h; i++)
        elements[i] = gen() % 2;
}

int MarkImage::Elem(int x, int y) const { return elements[y * w + x]; }

void MarkImage::SetElem(int x, int y, int val) { elements[y * w + x] = val; }

bool MarkImage::HasElem(int x, int y) const {
    return 0 <= x && x < w && 0 <= y && y < h;
}

void MarkImage::Print() const {
    for (int i = 0; i < w * h; i++) {
        if (i % w == 0)
            cout << "\n";
        cout << elements[i] << "\t";
    }
}

bool MarkImage::operator==(const MarkImage &other) const {
    if (w != other.w || h != other.h)
        return false;

    for (int i = 0; i < w * h; i++)
        if (elements[i] != other.elements[i])
            return false;

    return true;
}

MarkImage FirstMark(const MarkImage &original, int beginMark) {
    int mark = beginMark;
    MarkImage res(original);

    for (int y = 0; y < original.H(); y++) {
        for (int x = 0; x < original.W(); x++) {
            if (original.HasElem(x - 1, y) && original.Elem(x - 1, y) == 1 &&
                original.Elem(x, y) == 1) {
                res.SetElem(x, y, res.Elem(x - 1, y));
            } else if (original.HasElem(x, y - 1) &&
                       original.Elem(x, y - 1) == 1 &&
                       original.Elem(x, y) == 1) {
                res.SetElem(x, y, res.Elem(x, y - 1));
            } else if (original.Elem(x, y) == 1) {
                res.SetElem(x, y, mark);
                mark++;
            }
        }
    }
    return res;
}

MarkImage SecondMark(const MarkImage &img) {
    int allMarkCount = img.W() * img.H() / 2 + 1;

    vector<vector<pair<int, int>>> marksElements;
    marksElements.reserve(allMarkCount + 1);
    for (int i = 0; i <= allMarkCount; i++) {
        vector<pair<int, int>> mark;
        marksElements.push_back(mark);
    }

    set<pair<int, int>> mergePairsSet;
    vector<pair<int, int>> mergePairs;

    for (int x = 0; x < img.W(); x++) {
        for (int y = 0; y < img.H(); y++) {
            if (img.Elem(x, y) != 0) {
                // Put coords into mark
                marksElements[img.Elem(x, y)].push_back(std::make_pair(x, y));
            }

            if (img.HasElem(x, y - 1)) {
                int upElem = img.Elem(x, y - 1);
                int curElem = img.Elem(x, y);
                if (upElem != 0 && curElem != 0 && upElem != curElem)
                    mergePairsSet.insert(std::make_pair(curElem, upElem));
            }
        }
    }

    // Convert set to vector
    for (auto pair : mergePairsSet)
        mergePairs.push_back(pair);

    for (int i = 0; i < mergePairs.size(); i++) {
        int mergeToMark = mergePairs[i].second;
        int mergeFromMark = mergePairs[i].first;
        if (mergeToMark != mergeFromMark) {
            // Change marks of elements with mark <mergeFromMark> to
            // <mergeToMark>
            for (int i = 0; i < marksElements[mergeFromMark].size(); i++)
                marksElements[mergeToMark].push_back(
                    marksElements[mergeFromMark][i]);

            marksElements[mergeFromMark].clear();
        }

        // Change mark pairs which been changed
        for (int j = 0; j < mergePairs.size(); j++) {
            if (mergePairs[j].first == mergeFromMark)
                mergePairs[j].first = mergeToMark;

            else if (mergePairs[j].second == mergeFromMark)
                mergePairs[j].second = mergeToMark;
        }
    }

    // Start numerate again
    int newMark = 0;
    MarkImage res(img);
    for (int i = 0; i < marksElements.size(); i++) {
        if (marksElements[i].empty() == false)
            newMark++;
        for (int j = 0; j < marksElements[i].size(); j++)
            res.SetElem(marksElements[i][j].first, marksElements[i][j].second,
                        newMark);
    }
    return res;
}

MarkImage MarkParal(const MarkImage &original) {
    int rank, processors;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processors);

    if (rank == 0) {
        int otherProcRows = original.H() / processors;
        int mainProcRows = otherProcRows + original.H() % processors;

        for (int i = 1; i < processors; i++) {
            // Each process have unique marks
            int procBeginMark =
                (mainProcRows + otherProcRows * (i - 1)) * original.W() / 2 + 1;
            int beginY = otherProcRows * (i - 1) + mainProcRows;
            int width = original.W();
            int height = otherProcRows;
            int *data = new int[width * height];
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++)
                    data[y * width + x] = original.Elem(x, beginY + y);

            MPI_Send(&procBeginMark, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&width, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(data, original.W() * otherProcRows, MPI_INT, i, 0,
                     MPI_COMM_WORLD);
        }

        // Copy part of original
        MarkImage part(original.W(), mainProcRows);
        for (int y = 0; y < part.H(); y++)
            for (int x = 0; x < part.W(); x++)
                part.SetElem(x, y, original.Elem(x, y));

        MarkImage handledPart = FirstMark(part, 1);

        MarkImage fullHandledImage(original.W(), original.H());

        for (int y = 0; y < handledPart.H(); y++)
            for (int x = 0; x < handledPart.W(); x++)
                fullHandledImage.SetElem(x, y, handledPart.Elem(x, y));

        for (int i = 1; i < processors; i++) {
            int *data = new int[otherProcRows * original.W()];
            MPI_Recv(data, otherProcRows * original.W(), MPI_INT, i, 0,
                     MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            int beginY = otherProcRows * (i - 1) + mainProcRows;
            for (int y = 0; y < otherProcRows; y++)
                for (int x = 0; x < original.W(); x++)
                    fullHandledImage.SetElem(x, beginY + y,
                                             data[y * original.W() + x]);
        }

        return SecondMark(fullHandledImage);

    } else {
        int w, h, beginMark;
        MPI_Recv(&beginMark, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        MPI_Recv(&w, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&h, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        int *data = new int[w * h];
        MPI_Recv(data, w * h, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        // Convert data to image
        MarkImage localImage(w, h);
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
                localImage.SetElem(x, y, data[y * w + x]);

        // Handle image
        MarkImage localRes = FirstMark(localImage, beginMark);

        // Convert image to data
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
                data[y * w + x] = localRes.Elem(x, y);

        MPI_Send(data, w * h, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return MarkImage(1, 1);
}

bool CheckMark(const MarkImage &checkable) {
    for (int y = 0; y < checkable.H(); y++) {
        for (int x = 0; x < checkable.W(); x++) {
            if (checkable.Elem(x, y) == 0)
                continue;
            else if (checkable.HasElem(x, y - 1) &&
                     checkable.Elem(x, y - 1) != 0 &&
                     checkable.Elem(x, y) != checkable.Elem(x, y - 1))
                return false;
            else if (checkable.HasElem(x, y + 1) &&
                     checkable.Elem(x, y + 1) != 0 &&
                     checkable.Elem(x, y) != checkable.Elem(x, y + 1))
                return false;
            else if (checkable.HasElem(x - 1, y) &&
                     checkable.Elem(x - 1, y) != 0 &&
                     checkable.Elem(x, y) != checkable.Elem(x - 1, y))
                return false;
            else if (checkable.HasElem(x + 1, y) &&
                     checkable.Elem(x + 1, y) != 0 &&
                     checkable.Elem(x, y) != checkable.Elem(x + 1, y))
                return false;
        }
    }
    return true;
}
