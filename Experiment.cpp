//Copyright 2019 Shewa00


#include "Experiment.h"
#include <utility>

Experiment::Experiment(int min, int max) {
    buffer_sizes.reserve(log2(max / min) + 3);
    min /= 2;
    while (min <= max * 3 / 2) {
        buffer_sizes.push_back(min);
        min *= 2;
    }
    buffer_sizes.push_back(max * 3 / 2);
}

void Experiment::Init() {
    delete[] buffer;
    buffer = new int[Amount];
    for (size_t i = 0; i < Amount; i++)
        buffer[i] = rand() % 1000;
}

void Experiment::WarmUp() {
    int temp = 0;
    for (size_t i = 0; i < Amount; i++)
        temp = buffer[i];
}

void Experiment::Forward() {
    int temp = 0;
    for (size_t i = 0; i < Amount; i++) {
        temp = buffer[i];
    }
    temp++;
}

void Experiment::Backward() {
    int temp = 0;
    for (size_t i = Amount; i >= 1; i--) {
        temp = buffer[i - 1];
    }
    temp++;
}

void Experiment::Random() {
    int temp = 0;
    for (size_t i = 0; i < Amount; i++) {
        temp = buffer[rand() % Amount];
    }
    temp++;
}


void Experiment::Calculation() {
    std::string result;
    int iter = 0;
    for (auto i : buffer_sizes) {
        iter++;
        Amount = i * 1024 / sizeof(int);
        Init();
        WarmUp();
        auto clock_start = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < loop_amount; j++)
            Forward();
        auto clock_end = std::chrono::high_resolution_clock::now();
        auto clock_time = std::chrono::duration_cast<std::chrono::milliseconds>
                (clock_end - clock_start).count();
        Notes.emplace_back("Forward", iter, i, clock_time);

        clock_start = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < loop_amount; j++)
            Backward();
        clock_end = std::chrono::high_resolution_clock::now();
        clock_time = std::chrono::duration_cast<std::chrono::milliseconds>
                (clock_end - clock_start).count();
        clock_time = std::chrono::duration_cast<std::chrono::milliseconds>
                (clock_end - clock_start).count();
        Notes.emplace_back("Backward", iter, i, clock_time);


        clock_start = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < loop_amount; j++)
            Random();
        clock_end = std::chrono::high_resolution_clock::now();
        clock_time = std::chrono::duration_cast<std::chrono::milliseconds>
                (clock_end - clock_start).count();
        Notes.emplace_back("Random", iter, i, clock_time);
    }
    Amount = iter;
    std::ofstream out;          // поток для записи
    // окрываем файл для записи
    out.open("/Users/macbook/CLionProjects/lab-02/results.txt");
    if (out.is_open())
        this->print(out);
    GraphBuild();
}


void Experiment::print(std::ofstream &out) {
    out << "investigation:\n\ttravel_variant: Forward\n\texperiments:";
    for (auto i : Notes) {
        if (i.method == "Forward")
            i.print(out);
    }

    out << "\ninvestigation:\n\ttravel_variant: Backward\n\texperiments:";
    for (auto i : Notes) {
        if (i.method == "Backward")
            i.print(out);
    }

    out << "\ninvestigation:\n\ttravel_variant: Random\n\texperiments:";
    for (auto i : Notes) {
        if (i.method == "Random")
            i.print(out);
    }
}


void Experiment::GraphBuild() {
    std::string result = R"(<html>
        <head>
          <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
            <script type="text/javascript">
              google.charts.load('current', {'packages':['corechart']});
              google.charts.setOnLoadCallback(drawChart);
            function drawChart() {
              var data = new google.visualization.DataTable();
              data.addColumn('number', 'X');
         data.addColumn('number', 'forward');
        data.addColumn('number', 'backward');
         data.addColumn('number', 'random');
        )";

    std::vector<int64_t> forwardTime;
    std::vector<int64_t> backwardTime;
    std::vector<int64_t> randomTime;
    for (auto i : Notes) {
        if (i.method == "Forward")
            forwardTime.push_back(i.result);
        if (i.method == "Backward")
            backwardTime.push_back(i.result);
        if (i.method == "Random")
            randomTime.push_back(i.result);
    }

    result += "\ndata.addRows([\n";
    for (size_t i = 0; i < Amount; i++) {
        result += "[" + std::to_string(buffer_sizes[i]) + ", ";
        result += std::to_string(forwardTime[i]) + ", ";
        result += std::to_string(backwardTime[i]) + ", ";
        result += std::to_string(randomTime[i]) + ", ";

        result += "],\n";
    }
    result += "]);\n";

    result += R"(
              var options = {
                chart: {
                  title: 'CPU cache',
                },
                hAxis: {
                  title: 'Buffer size, KB',
                  logScale: false
                },
                vAxis: {
                  title: 'Time, milliseconds',
                  logScale: false
                },
                explorer: {
                  axis: 'horizontal',
                  keepInBounds: true,
                  maxZoomIn: 8.0,
                },
                width: 1200,
                height: 800,
              };
              var chart = new google.visualization.LineChart(document.getElementById('line_top_x'));
              chart.draw(data, options);
            }
          </script>
        </head>
        <body>
          <div id="line_top_x"></div>
        </body>
        </html>
        )";

    std::ofstream out;
    out.open("/Users/macbook/CLionProjects/lab-02/graph.html");
    if (out.is_open())
        out << result << std::endl;
}

Note::Note(std::string Method, int Number, int Buffer_size, int Result) {
    method = std::move(Method);
    number = Number;
    buffer_size = Buffer_size;
    result = Result;
}

void Note::print(std::ofstream &out) {
    out << "\n\t- experiment:\n\t\tnumber: " << number
        << "\n\t\tinput_data:\n\t\t\tbuffer_size: " << buffer_size
        << "\n\t\tresults:\n\t\t\tduration: " << result;
}
