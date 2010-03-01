#include "randomName.h"
#include "random.h"

string frenchf [346] = {"Adélaïde", "Adèle", "Adrienne", "Agathe", "Agnès", "Aile", "Aimée", "Alaine", "Albane", "Alette", "Alexandra", "Alexanne", "Alexis", "Alice", "Aline", "Alix", "Alizée", "Amalie", "Amandine", "Ambre", "Amée", "Amélie", "Anabelle", "Anaëlle", "Anaïs", "Andréanne", "Andrée", "Andréa", "Andrie", "Ange", "Angélique", "Annabelle", "Anne", "Anne-Marie", "Anne-Sophie", "Annette", "Annick", "Annie", "Antoinette", "Appoline", "Ariane", "Arielle", "Arlette", "Armelle", "Astérie", "Aubine", "Aurélie", "Aurore", "Avril", "Axelle", "Babette", "Barbe", "Béatrice", "Belle", "Bernadette", "Bernice", "Bertille", "Bien", "Aimée", "Blaisotte", "Blanche", "Blanchette", "Bluette", "Brigitte", "Calanthe", "Camille", "Capucine", "Carolane", "Carole", "Caroline", "Cassandre", "Catherine", "Cécilie", "Celeste", "Célia", "Céline", "Cerise", "Chanel", "Chantal", "Charline", "Charlotte", "Charmaine", "Cherie", "Chloë", "Christelle", "Christiane", "Christine", "Claire", "Claire-Marie", "Clarisse", "Claude", "Claudette", "Claudie", "Claudine", "Clémence", "Clémentine", "Clervie", "Cloé", "Clothilde", "Clotilde", "Colette", "Coline", "Colombe", "Constance", "Coralie", "Cordélie", "Corentine", "Corinne", "Cornélie", "Cosette", "Crescence", "Danielle", "Daphné", "Daphnée", "Delphine", "Denise", "Desirée", "Dianne", "Dionne", "Dominique", "Doralice", "Doré", "Dorette", "Doriane", "Dorothée", "Drucilla", "Égérie", "Elaine", "Éléanore", "Eléna", "Éliane", "Élisabeth", "Elise", "Élizabeth", "Élodie", "Eloise", "Emeline", "Émilie", "Émilienne", "Emmanuelle", "Emy", "Ernestine", "Esmé", "Estée", "Estelle", "Eugénie", "Eulalie", "Evangeline", "Eve", "Evelyne", "Fabienne", "Fanette", "Fanny", "Fantine", "Fernande", "Fifi", "Flamine", "Fleur", "Florence", "Florie", "Francine", "Françoise", "Frédérique", "Gabrielle", "Geneviève", "Georgette", "Georgine", "Germaine", "Gervaise", "Ghislaine", "Ginette", "Gisèle", "Grâce", "Hélène", "Héloïse", "Henriette", "Hermine", "Hyacinthe", "Inés", "Irène", "Iris", "Isabeau", "Isabelle", "Jacqueline", "Janine", "Jeand'arc", "Jeanette", "Jeanine", "Jeanne", "Jeanne-Aimée", "Jeanne-Yvette", "Jeannine", "Jenevieve", "Joëlle", "Josephine", "Josette", "Josiane", "Julie", "Juliette", "Justine", "Karine", "Laetitia", "Laure", "Laurence", "Lauriane", "Laurianne", "Laurie", "Laurine", "Léa", "Léonie", "Léonore", "Liane", "Lianne", "Liliane", "Lilianne", "Lise", "Lisette", "Loraine", "Loréline", "Lorraine", "Louise", "Lucie", "Lucienne", "Lucile", "Lucille", "Lucrèce", "Ludivine", "Lydie", "Mabelle", "Madeleine", "Maelle", "Magalie", "Manon", "Marcelle", "Margaux", "Margot", "Marguerite", "Marianne", "Marie", "Marie-Anne", "Marie-Claire", "Marie-Claude", "Marie-Ève", "Marie-France", "Marie-Hélène", "Marie-Laure", "Marielle", "Marie-Louise", "Marie-Noelle", "Marie-Pier", "Marie-Pierre", "Marie-Sophie", "Marie-Sylvie", "Marie-Thérèse", "Marine", "Marion", "Marjolaine", "Marthe", "Martine", "Mathilde", "Maude", "Maxime", "Maxine", "Mégane", "Mélaine", "Mélanie", "Mélina", "Mélissa", "Mélodie", "Michèle", "Micheline", "Mignon", "Mimi", "Minerve", "Mirabelle", "Mireille", "Monette", "Monique", "Morgane", "Musette", "Mylène", "Myriam", "Nadège", "Nadine", "Nanette", "Natalie", "Nathalie", "Nicole", "Nicolette", "Noelle", "Noemi", "Noémie", "Océane", "Odette", "Odile", "Ombeline", "Ondine", "Ophélie", "Orégane", "Patrice", "Paulette", "Pauline", "Philippine", "Quitterie", "Rachelle", "Raymonde", "Renée", "Rochelle", "Romaine", "Romane", "Rosalie", "Rose", "Rosine", "Roxane", "Sabine", "Sabrine", "Salome", "Salomé", "Sandrine", "Sara", "Séraphine", "Sidonie", "Simone", "Simonne", "Sixtine", "Solange", "Solène", "Solenne", "Sophie", "Stéphanie", "Suzanne", "Sylvie", "Thérèse", "Tiphaine", "Valentine", "Valérie", "Véronique", "Victoire", "Violette", "Virgie", "Virginie", "Vivienne", "Yannick", "Yvette", "Yvonne", "Zoëu"};
string frenchm [340] = {"Achille", "Adam", "Adolphe", "Adrien", "Aïdan", "Alain", "Alain-René", "Alban", "Albert", "Alexandre", "Alexis", "Aloïs", "Alphonse", "Amaury", "Ambroise", "Amédée", "Anatole", "André", "Anicet", "Anselme", "Antoine", "Aristide", "Armand", "Arnaud", "Arsène", "Arthur", "Aubert", "Aubin", "Audefroy", "Auguste", "Augustin", "Aurélien", "Aymeric", "Baptiste", "Bardiou", "Barthélémy", "Basile", "Bastien", "Baudier", "Baudouin", "Beau", "Benjamin", "Benoît", "Bérenger", "Bernard", "Bertrand", "Blaise", "Boudreaux", "Brice", "Brieux", "Bruno", "Cécil", "Cédric", "Cédrick", "Céléstin", "César", "Charles", "Charles-Antoine", "Charles", "Étienne", "Chrétien", "Christophe", "Clair", "Claude", "Clément", "Clovis", "Côme", "Constantin", "Corentin", "Cyprien", "Cyriaque", "Cyrille", "Damien", "Danick", "Daniel", "Dany", "Dartagnan", "David", "Denis", "Déreck", "Didier", "Dominique", "Donatien", "Edmond", "Édouard", "Égide", "Élias", "Élie", "Éliot", "Éloi", "Émerick", "Emeril", "Émile", "Émilien", "Emmanuel", "Enrick", "Éric", "Esmé", "Étienne", "Eugène", "Evarist", "Évrard", "Fabien", "Fabrice", "Faust", "Félix", "Félix", "Antoine", "Fernand", "Fidèle", "Firmin", "Flavien", "Florent", "Florian", "Foudil", "Franck", "François", "Frédéric", "Frédérick", "Gabin", "Gabriel", "Gaël", "Gaétan/Gaëtan", "Gaspard", "Gaston", "Gatien", "Gauthier", "Gautier", "Gédéon", "Geoffroy", "George-Marie", "Georges", "Gérard", "Germain", "Ghislain", "Gilbert", "Giles", "Gilles", "Giraud", "Grégoire", "Guillaume", "Gustave", "Guy", "Henri", "Hercule", "Hervé", "Hilaire", "Honoré", "Hubert", "Hugo", "Hugues", "Ignace", "Irène", "Isidore", "Ismaël", "Jacques", "Jacquot", "Jacques-Yves", "Jean", "Jean-Albert", "Jean-André", "Jean-Baptise", "Jean-Christophe", "Jean-Claude", "Jean-Didier", "Jean-France", "Jean-François", "Jean-Guy", "Jean-Jacques", "Jean-Jérôme", "Jean-Lou", "Jean-Louis", "Jean-Luc", "Jean-Marc", "Jean-Marie", "Jean-Michel", "Jean-Paul", "Jean-Philippe", "Jean-Pierre", "Jean-René", "Jean-Sébastien", "Jean-Simon", "Jean-Xavier", "Jean-Yves", "Jérémi", "Jérémie", "Jérémy", "Jermaine", "Jérôme", "Jessy", "Joël", "Joscelin", "Joseph", "Joseph-Benoît", "Jourdain", "Jules", "Julien", "Juste", "Justin", "Laurent", "Lazare", "Léandre", "Léo", "Léon", "Léonard", "Léopold", "Lilian", "Loïc", "Lothaire", "Lou", "Louis", "Louis-Charles", "Louis-Joseph", "Louis", "Philippe", "Louka", "Luc", "Lucas", "Lucien", "Ludovic", "Macaire", "Maël", "Marc", "Marc-Alexandre", "Marc", "André", "Marc-André", "Marc", "Antoine", "Marc-Antoine", "Marcel", "Marcellin", "Marc-Henri", "Marc", "Olivier", "Martial", "Martin", "Mathias", "Mathieu", "Mathis", "Mattéo", "Matthieu", "Maurice", "Maxence", "Maxime", "Maximilien", "Michel", "Mickaël", "Mortimer", "Narcisse", "Nicéphore", "Nicolas", "Noé", "Noël", "Nordine", "Octavien", "Odilon", "Olivier", "Pascal", "Patrice", "Paul", "Philippe", "Pier-Olivier", "Pierre", "Pierrot", "Pierre", "Alexandre", "Pierre-Luc", "Pierre", "Olivier", "Pierrick", "Quentin", "Rafaël", "Raoul", "Raphaël", "Rayan", "Raymond", "Réal", "Régis", "Réjean", "Rémi", "Rémy", "Renald", "Renaud", "René", "Richard", "Robert", "Rodolphe", "Rodrigue", "Roger", "Roland", "Romain", "Romaine", "Samuel", "Samson", "Samy", "Sébastien", "Serge", "Séverin", "Simeon", "Simon", "Olivier", "Sofiane", "Stanislas", "Stefan-André", "Stephan", "Stéphane", "Sulpice", "Sylvain", "Sylvestre", "Tancrede", "Terence", "Théo", "Théodore", "Théophile", "Thibaud", "Thibault", "Thibaut", "Thierry", "Thomas", "Timothé", "Timothée", "Toussaint", "Valentin", "Valère", "Venant", "Victor", "Vincent", "Virgile", "Voltaire", "Xavier", "Yan", "Yanick", "Yanis", "Yoan", "Yohan", "Yves", "Yvon", "Zacharie", "Zephrinu"};
string japanesef [22] = {"Yohko", "Megumi", "Sakura", "Hanako", "Ai", "Hirano", "Takako", "Nana", "Izumi", "Aki", "Yuki", "Yoshiko", "Aya", "Yuri", "Hina", "Rina", "Yuuna", "Yukiko", "Mai", "Aoi", "Nanase", "Natsumi"};
string japanesem [33] = {"Risuke", "Ichirou", "Heisuke", "Hajime", "Tarou", "Keisuke", "Hideki", "Shuusuke", "Touya", "Minoru", "Mitsuo", "Tetsuji", "Hideo", "Shuuji", "Shinnosuke", "Mitsunori", "Hirotaro", "Jun'ichi", "Kazuo", "Hiroshi", "Masatoshi", "Hitoshi", "Akira", "Hiroto", "Ren", "Yuto", "Satoshi", "Kei", "Hiroki", "Kenjirou", "Kenshirou", "Kenji", "Tatsuhiko"};
string evilTitles [18] = {"Baron", "Bringer", "Champion", "Demon", "Duke", "Emperor", "Empress", "Harbinger", "Heart", "Herald", "Lady", "Lord", "Master", "Mother", "Overlord", "Queen", "Servant", "Spawn"};
string evilStuff [20] = {"Annihilation", "Conquest", "Darkness", "Decay", "Despair", "Destruction", "Doom", "Dread", "Famine", "Fear", "Fury", "Greed", "Hate", "Horror", "Madness", "Malice", "Pestilence", "Ruin", "Sorrow", "Terror"};

randomNameGenerator::randomNameGenerator()
{
}

string randomNameGenerator::getRandomName(int nameStyle)
{
    if ( nameStyle == 1 ) {
		return frenchf[Random::integer(sizeof(frenchf) / sizeof(string))];
    }
    if ( nameStyle == 2 ) {
        return frenchm[Random::integer(sizeof(frenchm) / sizeof(string))];
    }
    if ( nameStyle == 3 ) {
		return japanesef[Random::integer(sizeof(japanesef) / sizeof(string))];
    }
    if ( nameStyle == 4 ) {
        return japanesem[Random::integer(sizeof(japanesem) / sizeof(string))];
    }
    if ( nameStyle == 5 ) {
        return (evilTitles[Random::integer(sizeof(evilTitles) / sizeof(string))] +
			" of " +
			evilStuff[Random::integer(sizeof(evilStuff) / sizeof(string))]);
    }
    return "REPORT_ME";
}

string randomNameGenerator::getRandomName(const std::string& nameStyle)
{
    if ( nameStyle == "frenchf" ) {
        return getRandomName(1);
    }
    if ( nameStyle == "frenchm" ) {
        return getRandomName(2);
    }
    if ( nameStyle == "japanesef" ) {
        return getRandomName(3);
    }
    if ( nameStyle == "japanesem" ) {
        return getRandomName(4);
    }
    if ( nameStyle == "evilepithet" ) {
        return getRandomName(5);
    }
    return getRandomName(0);
}

randomNameGenerator::~randomNameGenerator()
{

}
