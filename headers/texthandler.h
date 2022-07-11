#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

static const std::string TEXTS_DIR ("../Texts/");

std::vector<const char*> EvaluationTexts_short{
    "C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do, it blows away your whole leg.",

	"The traditional mathematician recognizes and appreciates mathematical elegance when he sees it. I propose to go one step further, and to consider elegance an essential ingredient of mathematics: if it is clumsy, it is not mathematics.",

	"If you're always worried about crushing the ants beneath you... you won't be able to walk.",

	"#include <stdio.h>\nint main()\n{\nint x = 10;\nfor (int i = 0; i < x; i ++)\nprintf('Hello World ');\nreturn 0;\n}",

	"You'll hunt me. You'll condemn me, set the dogs on me. Because that's what needs to happen. Because sometimes the truth isn't good enough. Sometimes people deserve to have their faith rewarded."
};

std::vector<const char*> EvaluationTexts_medium{
    "Velocity is one of a kind of typing speed evaluators. Run from the Windows command prompt, it allows for a unique command line experience of practicing and honing your typing speed and skills without the distractions, ads and javascript bloat that you might experience from any browser based evaluator.",

	"bucket established circulation maze loss option dribble salvation play examination miscarriage lead trick squash moving motivation punish aware intelligence harsh reputation race cancer source floor quality weakness crew lake aisle trouble remember modest reverse kick pardon retire scatter unit earwax document arise cheat autonomy",

	"Everything assumes such an immense quality when you are a child. You are a seed amongst the trees - waiting, waiting, waiting - for the soil and the sun to open you up, to release you from the feeling of smallness. And in that smallness rests another seed, one of fear or something like it - a lack of agency and hopelessness. ",
	
	"Written over half a century ago, Business Adventures remains relevant in its examination of the world of Wall Street. The book is a collection of stories written over John Brooks' career. Brooks excelled at writing insightful commentary about complicated business phenomenon in a way that was accessible to the everyday reader.",

	"Blood-red snakeclouds gathered in the western sky, and the rubber trees glowed as if on fire. Lu Beiping counted the pits he'd dug that day on the recently denuded hillside, picked up the squad leader's notebook, recorded his number, and stood scanning the list of names for Fong's mark. Nothing. She'd vanished."
};

std::vector<const char*> EvaluationTexts_long{
    "My name is Yoshikage Kira. I'm 33 years old. My house is in the northeast section of Morioh, where all the villas are, and I am not married. I work as an employee for the Kame Yu department stores, and I get home every day by 8 PM at the latest. I don't smoke, but I occasionally drink. I'm in bed by 11 PM, and make sure I get eight hours of sleep, no matter what. After having a glass of warm milk and doing about twenty minutes of stretches before going to bed, I usually have no problems sleeping until morning. Just like a baby, I wake up without any fatigue or stress in the morning.",

	"I remember being reborn a thousand times.\nIn the between-times, and at the very start and end of a life, the memories are strong. I remember the warmth of the sun on my crumpled wings as they unfold for one brilliant bright day of flight. I relive the elation of the chase and the sinking of my teeth into the throat of my prey- what a joy it is to satisfy physical hunger. I recall the agony of being prey, because of course, all things must balance out in the end. My heart has pumped cold blood through sinuous coils and thumped its warm-blooded rhythm from ocean depths.\nLike you, I am the sum of a million experiences.\nIt's true that the messy, bloody rebirth into the world as a human stuns me every time. The shock of union as soul re-joins body. The violent expulsion through the birth canal. The startling sensation of air on skin, after an age of ethereal floating. The instinctive gasp, that first mortal wail. And so it begins again, and I'm here at the start of a new life.",

	"The Long Flight Home draws on the historical importance of carrier pigeons to craft a masterful novel about strength and hope in the midst of wartime crisis. The story focuses on Susan Shepherd, a young British woman who was orphaned as a child. Susan was raised by her grandfather Bertie, who taught her the art of raising homing pigeons. As the war gets closer, threatening everything she's ever known, Susan becomes involved in a covert mission to air-drop hundreds of homing pigeons in German-occupied France.",

	"Once, a hare saw a tortoise walking slowly with a heavy shell on his back. The hare was very proud of himself and he asked the tortoise. 'Shall we have a race?'. The tortoise agreed. They started the running race. The hare ran very fast. But the tortoise walked very slowly. The proud hair rested under a tree and soon slept off. But the tortoise walked very fast, slowly and steadily and reached the goal. At last, the tortoise won the race.\nMoral : Pride goes before a fall.",

	"equal invent necessary courageous sofa sticky hate macho produce adorable spiritual clap temper mellow fact apparatus group unequaled modern imperfect trick challenge coat chivalrous snatch present hanging sassy peaceful funny meek curious perpetual teaching face receipt small judge meal idea fall pickle abiding glue acrid filthy annoyed nod like guarantee puffy anxious excite juvenile sea stimulating tire peep knowing swing sleep heavy lackadaisical suit drab bewildered satisfy tart current adhesive join brick gigantic scared boundless"
};


void LoadParasfromText (const char* filename) {
    std::ifstream fileHandle(TEXTS_DIR + filename);
    if (fileHandle.is_open()) {
        std::string para;
        while (std::getline(fileHandle, para)) {
            if (para.length() <= 200) {
                EvaluationTexts_short.push_back(para.c_str());
            } 
            else if (para.length() <= 400) {
                EvaluationTexts_medium.push_back(para.c_str());
            }
            else {
                EvaluationTexts_long.push_back(para.c_str());
            }
        }
        fileHandle.close();

    } else {
        std::cout<< "ERROR::Unable to open "<< filename<< "\n";
    }
}

void LoadPermutationsfromText (const char* filename, std::mt19937 &RandomEngine) {

    std::ifstream fileHandle(TEXTS_DIR + filename);
    std::vector< std::string > Words;
    Words.reserve(5000);

    if (fileHandle.is_open()) {
        std::string word;
        while (std::getline(fileHandle, word)) {
            Words.emplace_back(word);
        }
        fileHandle.close();

        std::uniform_int_distribution<int> RNG(0, Words.size());
        for (int i = 0; i < 10; i ++) {
            std::string Para("");
            for (int i = 0; i < 30; i ++) {
                int randidx = RNG(RandomEngine);
                Para.append(Words[randidx]);
                Para.push_back(' ');
            }
            Para.pop_back();
            EvaluationTexts_short.push_back(Para.c_str());
        }
        for (int i = 0; i < 7; i ++) {
            std::string Para("");
            for (int i = 0; i < 50; i ++) {
                int randidx = RNG(RandomEngine);
                Para.append(Words[randidx]);
                Para.push_back(' ');
            }
            Para.pop_back();
            EvaluationTexts_short.push_back(Para.c_str());
        }
        for (int i = 0; i < 5; i ++) {
            std::string Para("");
            for (int i = 0; i < 100; i ++) {
                int randidx = RNG(RandomEngine);
                Para.append(Words[randidx]);
                Para.push_back(' ');
            }
            Para.pop_back();
            EvaluationTexts_short.push_back(Para.c_str());
        }

    } else {
        std::cout<< "ERROR::Unable to open "<< filename<< "\n";
    }
}
/*
const char* LoadTextFromPath (const char* fullpath) {
    std::ifstream reader(fullpath);
    if (reader.fail()) {
        printf("ERROR:: Unable to open file at %s\n", fullpath);
        exit(1);
    }
    std::stringstream textstream;
    textstream << reader.rdbuf();
    reader.close();

    std::string container = textstream.str();
    return container.c_str();
}

void SaveText (const char* text) {
    std::string custom_texts_path = TEXTS_DIR + std::string("Custom.txt");
    std::ofstream writer(custom_texts_path);
    if (writer.fail()) {
        printf("ERROR:: Error opening custom Texts' path\n");
        exit(1);
    }
    // Write into a file with a format (JSON/custom one)
}
*/