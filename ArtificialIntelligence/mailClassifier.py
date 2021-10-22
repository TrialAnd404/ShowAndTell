#!/usr/bin/env python


import math
import sys
import os
import argparse
from collections import Counter, defaultdict
import pickle
import nltk

import re
import csv

"""
    text Classifier
    -------------------------
    a small interface for document classification. Implement your own Naive Bayes classifier
    by completing the class NaiveBayestextClassifier below.
  
    utilizing:
    nltk:
        a tokenizing library that transforms single strings into token lists presumably using unicorn-magic
    sets:
        a set is a type of list, that can hold only one of each entry.
        a list [1,1,2,2,3,3] would result in the set [1,2,3]
        to get a set from a list, we call set(myList)
    dictionaries:
        a dictionary is a key-value list.
        essentially, this means that a dictionary might look like this:
        myDictionary {
            car : Toyota
            price : 10000
            mileage: 120000
        }
        but values are not restricted to just numbers or strings. it is possible to fabricate more elaborate
        dictionaries with sub-dictionaries as values for keys, for example:
        myDictionary {
            Vanessa : {
                car : Toyota
                price : 10000
                mileage : 120000
            }
            Maria : {
                car : BMW
                price : 20000
                mileage : 80000
            }
        }
        a single key can only be present ONCE in a dictionary (unique). as seen above, each subdirectory has only
        one key called car, the big surrounding directory has keys Vanessa and Maria.
        to access a value we use myDictionary.get(myKey).
        if we are not sure whether the key exists, we can use ...get.(myKey, defaultValue) to always be presented with
        a default value in case we have no such key (yet) in our dictionary.
"""


class NaiveBayestextClassifier:

    def __init__(self):
        # self.model = None
        """
        our program starts with a little bit of setup.
        if on program start there is a file called classifier.pkl,
        we read it and load our model from it.
        this only happens, if --train has been called previously.
        otherwise, this step is skipped
        """
        self.model = {}
        if os.path.isfile('classifier.pkl'):
            with open('classifier.pkl', 'rb') as file:
                self.model = pickle.load(file)

    def calculateChi(self, c1_w, c2_w, c1_not_w, c2_not_w):
        # print(c1_w, c2_w, c1_not_w, c2_not_w)
        """
        this function is used to calculate the 2x2matrix and the according chi-squared value of a word.
        it accepts the values of the matrix by row, meaning:

            c1_w    |   c2_w
                    |
            -------------------
            c1_not_w| c2_not_w
                    |

            where c1_w is the amount of ham-mails containing the word w, and c1_not_w is the amount
            of ham-mails NOT containing the word w.
            c2_w and c2_not_w are essentially the same metric, but concerning spam instead of ham mails.

            in order to keep the calculation readable and close to the paper,
            numerous helping variables are calculated first, which is not necessary in theory.
            after that the calculation of the chi-squared value is done as shown in the paper.

            for testing, the example matrix from the paper was used, replicating the chi-squared value of ~8.10.

            we can also see, that tokens that appear in all spam and ham mails will essentially break
            the formula. we COULD try to mitigate this with checks:
            basically if a column sum or a row sum is zero (-> calculation would fail) we just return 0 as our chi value
            since this occurs only for the token "subject" in this case, this step is skipped
        """
        W1 = c1_w + c2_w
        W2 = c1_not_w + c2_not_w
        C1 = c1_w + c1_not_w
        C2 = c2_w + c2_not_w
        N = W1 + W2

        M = [[c1_w, c2_w], [c1_not_w, c2_not_w]]

        W = [W1, W2]
        C = [C1, C2]

        chi = 0
        """
        this is bascially the formula given on the paper. for all column sums, calculate chi value with all row sums
        -> we iterate through both in 2 for loops.
        """
        for i in range(len(M)):
            for j in range(len(M[0])):
                E = (W[i] * C[j]) / N
                chi += (M[i][j] - E) ** 2 / E

        return chi

    # classifier.train(tokens_all_classes: token->amount, tokens_specific_genre: genre->token->amount])
    def train(self, spamMails, hamMails):
        """
        :param spamMails : the list of (previoulsy tokenizend + lowercased + set-ed) spam mails
        :param hamMails : the list of (previoulsy tokenizend + lowercased + set-ed) ham mails

        this function serves as the main training function in which the model for the classifier is generated & saved.
        in order to save it in the general directory, where this script is located aswell, we must first move 3 steps
        down, as the initial --train call moves to the training data which is 3 directories deep.
        """
        os.chdir('../../../')  # we move down 3 steps
        location = os.getcwd()  # and reset our locatiom to where we are now
        print("saving model to ", location)
        """
        we want to know with which words we are working with, for that we use a set,
        in which every value can only occur once.
        
        this serves the purpose of getting rid of duplicates:
            mail1 : "i am an apple"
            mail2 : "i am a horse"

            these mails will result in a bag of words like this:
                i, am, an, apple, a, horse
                => even though "i" and "am" occur in both mails, they only get added to our bag of words once.

                NOTE: we will later filter the entries of our bag of words.
                some tokens will contain letters we dont want to use, like punctuation or special characters.
                this makes for a pretty big performance overhead tha could 
                be adressed after finalizing the classifier and is a worthy TODO
        """
        bagOfWords = set()

        """
        we are using a dictionary for our model.
        the rough shape of our model will look something like this:

             classifierModel[
                  spam_count : 
                  -> total number of spam mails
                  ham_count :
                  -> total number of ham mails  
                  priors [
                      spam : 
                      -> the calculated spam prior
                      (spamMails / totalMails)
                      ham :
                      -> the calculated ham prior
                      (hamMails / totalMails)
                  ] 
                  vocabulary[
                      token1[
                          ham_occurences :
                          -> amount of hamMails containing this token   
                          spam_occurences :
                          -> amount of spamMails containing this token  
                          total : 
                          -> total occurences of thia token
                          p_ham :
                          -> possibility of this token occuring in a hamMail
                          p_spam :
                          -> possibility of this token occuring in a spamMail
                          chi : 
                          -> the chi value of this token
                      ]
                      token2[

                      ... see above

                      ]

                      ...

                  ]
             ]

             as shown above, the model is a dictionary with several sub-dictionaries
        """
        classifierModel = defaultdict(dict)

        classifierModel["spam_count"] = len(spamMails)
        classifierModel["ham_count"] = len(hamMails)
        classifierModel["priors"]["spam"] = classifierModel.get("spam_count") / (
                    classifierModel.get("spam_count") + classifierModel.get("ham_count"))
        classifierModel["priors"]["ham"] = classifierModel.get("ham_count") / (
                    classifierModel.get("spam_count") + classifierModel.get("ham_count"))

        """
        for unknown reasons, we have to instanciate our vocabulary dictionary seperately and later put it into our model
        """
        vocabulary = defaultdict(dict)
        """
        we now iterate the previously (immediately after reading the 'train' agrument)
        tokenized mails and add all tokens to our bag of words.
        NOTE: this is where we might be able to optimize some things in the future.
        we have code complexity O(n) for the creation of our bag of words,
        and then O(n²) for the creation of our vocabulary since we iterate both our generated bagOfWord 
        and our mails.
        
        this is straight up useless, but it works. What we could do, is instead of generating a bag of words that we
        later do not use anymore, just immediately put the token in our vocabulary.
        This would bypass the initial O(n) complexity of generating the bagOfWords.
        
        but again this solution makes for a little easier to read code.
        """
        for mail in hamMails:
            for token in mail:
                bagOfWords.add(token)
        for mail in spamMails:
            for token in mail:
                bagOfWords.add(token)

        """
        we now iterate our bag of words and check for letters other than a-z,
        if the token contains anything else it gets tossed and is subsequently ignored.
        this is so for example the token " 've " that occurs unproportionally often or the token " @ " get ignored,
        and we are left only with easy-to-read, lowercased tokens that do not contain special characters
        """
        for token in bagOfWords:
            if re.search("^[a-z]*$", token) and token != "subject":
                for mail in hamMails:
                    """
                    if a token is accepted, we then check if it comes from a spam or ham mail,
                    and increment the according value in our token accordingly.
                    this checkup is admittedly VERY slow, but it is easy to understand and read
                    
                    sidenote:
                        since we iterate all tokens, and then all mails,
                        the complexity of this step alone is O(n²)
                        (potentially n tokens * potentially n mails = n² complexity)
                    """
                    if token in mail:
                        vocabulary[token]["ham_occurences"] = vocabulary[token].get("ham_occurences", 0) + 1
                for mail in spamMails:
                    if token in mail:
                        vocabulary[token]["spam_occurences"] = vocabulary[token].get("spam_occurences", 0) + 1
                """
                last we calculate the total occurences of our token
                """
                vocabulary[token]["total"] = vocabulary[token].get("ham_occurences", 0) + vocabulary[token].get(
                    "spam_occurences", 0)

        for token in vocabulary.keys():
            """
            we have now successfully counted all out tokens, and can now calculate their 
            individual propabilities of being in a spam or ham mail.
            the formula is:
                (occurences in ham) / (number of ham mails)
                equivalent for spam
            """
            # print(token)
            vocabulary[token]["p_ham"] = vocabulary[token].get("ham_occurences", 0) / len(hamMails)
            vocabulary[token]["p_spam"] = vocabulary[token].get("spam_occurences", 0) / len(spamMails)

            """
            lastly we calculate the chi value of our token.
            we use:
                ...get("spam_occurences",0) to make sure we get at least the value 0,
                since some tokens might not have occured in spam mails,
                therefore they do not possess a key called "spam_occurences".
                this 'hacky' way ensures we always pass values to our chi function and do not break here.
            """
            # print(token)

            vocabulary[token]["chi"] = self.calculateChi(
                vocabulary[token].get("ham_occurences", 0),
                vocabulary[token].get("spam_occurences", 0),
                len(hamMails) - vocabulary[token].get("ham_occurences", 0),
                len(spamMails) - vocabulary[token].get("spam_occurences", 0)
            )

        """
        this lambda expression sorts our vocabulary into a list object by a value in its nested dictionary,
        as per the python3 documentation. example: https://careerkarma.com/blog/python-sort-a-dictionary-by-value/
        
        it is sorted in reverse, so the big values get sorted to the front.
        after this we can simply grab the first N values from the list.
        """
        sorted_vocabulary = sorted(vocabulary.items(), key=lambda x: (x[1]['chi']), reverse=True)
        N = 100
        print("CHI PLACEMENTS 1-100:")
        for i in range(N):
            print(sorted_vocabulary[i])
        print("CHI PLACEMENTS 101-200")
        for i in range(N):
            print(sorted_vocabulary[i + 100])
        print("CHI PLACEMENTS 201-300")
        for i in range(N):
            print(sorted_vocabulary[i + 200])
        print("place 300 for testing:")
        print(sorted_vocabulary[299])
        # various testing outputs
        # print(vocabulary["free"]
        # print(self.calculateChi(10,1,50,59))

        """
        finally our vocabulary is completely done, and we dump it in our model.
        why this does not work "natively" is beyond me.
        this little workaround solves all problems we have when trying to build the
        vocabulary in one step.
        """
        classifierModel["vocabulary"] = vocabulary
        classifierModel["100"] = sorted_vocabulary[:100]
        classifierModel["200"] = sorted_vocabulary[:200]
        classifierModel["300"] = sorted_vocabulary[:300]
        classifierModel["sorted"] = sorted_vocabulary
        # for token in classifierModel["100"]:
        # print(token[0])
        # print(classifierModel)
        """
        now we save our model to a binary file where we can read it when needed in the future
        """
        with open('classifier.pkl', 'wb') as file:
            pickle.dump(classifierModel, file)

    def test(self, spamMails, hamMails):
        print("test starting")

    def apply(self, spamMails, hamMails):
        """
        :param spamMails: a list containing all spam mails we want to classifiy
        :param hamMails: a list containing all ham mails we want to classifiy
        :return: empty

        the following declarations are a rather "useless" overhead that we dont necessarily need.
        since we want to use our classifier on several different sets of tokens and compare the results,
        it is nicer to have all values printed at the same time later. this, sadly, means that it is much more
        conventient to just declare a whole bunch of variables here, so we can later print them all at once.
        """
        correctSpam100 = 0
        correctHam100 = 0
        incorrectSpam100 = 0
        incorrectHam100 = 0
        correctSpam200 = 0
        correctHam200 = 0
        incorrectSpam200 = 0
        incorrectHam200 = 0
        correctSpam300 = 0
        correctHam300 = 0
        incorrectSpam300 = 0
        incorrectHam300 = 0
        correctSpamTotal = 0
        correctHamTotal = 0
        incorrectSpamTotal = 0
        incorrectHamTotal = 0

        total = 0
        totalSpam = 0
        totalHam = 0
        """
        since in theory, the propability of any token to be in a spam mail can never be 0, we introduce a "backup" valie called epsilon here.
        this is a so called superparameter we can adjust after/during training, to make the model more accurate.
        
        this superparameter has not been updated during training. we COULD automate this propably, by having the code
        run several times and adjust this parameter itself. 
        """
        epsilon = 0.0001


        """
        here starts the copy paste fiesta. it is simply a matter of laziness why this code has not been
        moved to a seperate function where it would look much cleaner.
        """
        ###### FIRST WE ITERATE THROUGH KNOWN HAM MAILS AND COMPARE RESULTS

        for mail in hamMails:
            """
            this for loop iterates over all hamMails and checks if a the single current mail is more
            likely to be a spam or a ham mail, by checking if the tokens in our vocabulary do or dont occur
            in the mail.
            
            the same happens later on with spamMails.
            """
            total += 1
            totalHam += 1
            p_spam = 0
            p_ham = 0

            """
            first we add the priors for spam and ham, since those get added anyways.
            
            since the numbers get too small when mulitplying propabilities 
            python would eventually simply round the result down to 0.
            instead we calculate in log space and just add the logarithmic values
            -> see: log(a*b) = log(a) + log(b)
            since we only care about the "bigger picture", to check if a mail is
            more likely to be spam over ham we can still check which of the values is bigger since:
            
                if
                    a*b < c*d           <-- "conventional" handling of propabilities
                it is also true that:
                    log(a*b) < log(c*d) <-- same calculation, but in log space
                    = log(a) + log(b) < log(c) + log (d)
                    
                thus, calculating in log space is beneficial for us
            """

            p_ham += math.log2(self.model["priors"]["ham"])
            p_spam += math.log2(self.model["priors"]["spam"])

            for entry in self.model["sorted"][:100]:
                """
                this loop (aswell as all other similar loops from here on) iterates over all selected tokens,
                in this case the best 100 tokens by chi value.
                """
                token = entry[0]

                """
                we check for all tokens if they occur in our mails we want to classify.
                if yes -> p_spam and p_ham of the current token get added to our propabilities,
                if not the opposite porpabilities, meaning 1-p_spam/ham get added
                """

                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )

            """
            our first 100 tokens have been iterated. we stop and check for each mail if the classification is indeed
            correct, and increment the respective helper variables declared at the start of the apply function.
            """
            # print("p_ham: %d, p_spam: %d" % (p_ham, p_spam))
            if p_spam > p_ham:
                # print("Prediction: Mail " + mail[1] + " is Spam")
                incorrectHam100 += 1
            else:
                # print("Prediction: Mail " + mail[1] + " is Ham")
                correctHam100 += 1
            # print("actual class: Ham")

            """
            now we want to see what the result would have been if we use the 200 best tokens. we already have a p_ham
            and p_spam for the best 100 tokens, so we do not need to reset here, we can just keep going and load the
            next 100 tokens.
            """
            for entry in self.model["sorted"][100:200]:
                token = entry[0]
                """
                same procedure as before. we check the new tokens and increment our p_ham / p_spam accordingly.
                """
                # print(p_ham)
                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )

            """
            the values of the tokens 100-200 have been added to the p_ham / p_spam values. we can now check again
            and track our results in seperate variables.
            """
            # print("p_ham: %d, p_spam: %d" % (p_ham, p_spam))
            if p_spam > p_ham:
                # print("Prediction: Mail " + mail[1] + " is Spam")
                incorrectHam200 += 1
            else:
                # print("Prediction: Mail " + mail[1] + " is Ham")
                correctHam200 += 1
            # print("actual class: Ham")
            for entry in self.model["sorted"][200:300]:
                token = entry[0]
                """
                same idea as before, we want the values for p_ham and p_spam for the best 300 tokens, we do already have
                results for the best 200, so we can just keep using those.
                """
                # print(p_ham)
                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )
            """
            again all tokens from 200-300 have been iterated, and we check if the current mail is more likely to be
            spam or ham.
            """
            # print("p_ham: %d, p_spam: %d" % (p_ham, p_spam))
            if p_spam > p_ham:
                # print("Prediction: Mail " + mail[1] + " is Spam")
                incorrectHam300 += 1
            else:
                # print("Prediction: Mail " + mail[1] + " is Ham")
                correctHam300 += 1
            # print("actual class: Ham")

            """
            as a bonus we now repeat the procedure for all remaining tokens
            """
            for entry in self.model["sorted"][300:]:
                token = entry[0]

                # print(p_ham)
                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )

            """
            the entirety of our known vocabuöary has been iterated and any token occuring
            (or not occuring) in our mail has contributed to a p_ham and p_spam value for our single current mail.
            """
            # print("p_ham: %d, p_spam: %d" % (p_ham, p_spam))
            if p_spam > p_ham:
                # print("Prediction: Mail " + mail[1] + " is Spam")
                incorrectHamTotal += 1
            else:
                # print("Prediction: Mail " + mail[1] + " is Ham")
                correctHamTotal += 1

        """
        
        now we repeat the procedure for all mails in our spam mail list to see if our classifier will indeed
        recognize them as spam mails.
        """
        ###### NOW WE ITERATE KNOWN SPAM MAILS & CHECK
        for mail in spamMails:
            total += 1
            totalSpam += 1
            p_spam = 0
            p_ham = 0

            """
            adding priors, setting up small debug variables..
            """

            p_ham += math.log2(self.model["priors"]["ham"])
            p_spam += math.log2(self.model["priors"]["spam"])

            for entry in self.model["sorted"][:100]:
                token = entry[0]
                """
                checking the first 100 tokens
                """
                # print(p_ham)
                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )

            """
            checking if mail is currently classified as spam (would be correct, since is a known spam mail)
            """
            # print("p_ham: %d, p_spam: %d" % (p_ham, p_spam))
            if p_spam > p_ham:
                # print("Prediction: Mail " + mail[1] + " is Spam")
                correctSpam100 += 1
            else:
                # print("Prediction: Mail " + mail[1] + " is Ham")
                incorrectSpam100 += 1
            # print("actual class: Ham")
            for entry in self.model["sorted"][100:200]:
                token = entry[0]
                """
                checking for tokens 100-200
                """
                # print(p_ham)
                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )

            if p_spam > p_ham:
                correctSpam200 += 1
            else:
                incorrectSpam200 += 1
            for entry in self.model["sorted"][200:300]:
                token = entry[0]

                """
                checking for tokens 200-300
                """

                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )

            if p_spam > p_ham:
                correctSpam300 += 1
            else:
                incorrectSpam300 += 1

            for entry in self.model["sorted"][300:]:
                token = entry[0]
                """
                checking for complete rest of tokens
                """
                # print(p_ham)
                if token in mail[0]:
                    p_ham += math.log2(max(self.model["vocabulary"][token]["p_ham"], epsilon))
                    p_spam += math.log2(max(self.model["vocabulary"][token]["p_spam"], epsilon))
                else:
                    p_ham += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_ham"], epsilon)
                    )
                    p_spam += math.log2(
                        max(1 - self.model["vocabulary"][token]["p_spam"], epsilon)
                    )

            # print("p_ham: %d, p_spam: %d" % (p_ham, p_spam))
            if p_spam > p_ham:
                # print("Prediction: Mail " + mail[1] + " is Spam")
                correctSpamTotal += 1
            else:
                # print("Prediction: Mail " + mail[1] + " is Ham")
                incorrectSpamTotal += 1
        """
        now, all mails have been dissected and checked against our vocabulary with multiple subsets.
        we now print our findings, containing the amount of correctly classified mails aswell as the
        amount of incorrectly classified mails.
        """
        print("100 best tokens classification:")
        print("correct: %d spam, %d ham" % (correctSpam100, correctHam100))
        print("incorrect: %d spam, %d ham" % (incorrectSpam100, incorrectHam100))
        print("200 best tokens classification:")
        print("correct: %d spam, %d ham" % (correctSpam200, correctHam200))
        print("incorrect: %d spam, %d ham" % (incorrectSpam200, incorrectHam200))
        print("300 best tokens classification:")
        print("correct: %d spam, %d ham" % (correctSpam300, correctHam300))
        print("incorrect: %d spam, %d ham" % (incorrectSpam300, incorrectHam300))
        print("all tokens classification:")
        print("correct: %d spam, %d ham" % (correctSpamTotal, correctHamTotal))
        print("incorrect: %d spam, %d ham" % (incorrectSpamTotal, incorrectHamTotal))

        """
        and now, we are finally done
        """

if __name__ == "__main__":

    """
    the main program that gets called when executing python3 classifier.py
    
    we have implemented a program that consumes arguments and acts accordingly.
    """
    parser = argparse.ArgumentParser(description='A document classifier.')
    parser.add_argument(
        '--train', help="train the classifier", action='store_true')
    """
    the argument "--test" is deprecated and was previously used to test the arg parser
    """
    parser.add_argument(
        '--test', help="train the classifier (alternative)", action='store_true')
    parser.add_argument('--apply', help="apply the classifier (you'll need to train or load"
                                        "a trained model first)", action='store_true')
    parser.add_argument('--inspect', help="get some info about the learned model",
                        action='store_true')

    """
    we read the argument and pass it to the variable called args
    """
    args = parser.parse_args()

    classifier = NaiveBayestextClassifier()

    """
    depending on our argument (stored in args variable) we call a different function
    """
    if args.train:
        """
        first we read both our spam and our ham mails.
        to do so, we switch our Current Working Directory (cwd) to the training dataset.
        """
        spamMails = []
        hamMails = []
        os.chdir('corpus-mails/corpus/training')
        location = os.getcwd()
        """
        now we read all files in our training location.
        all subdirs get opened, all files are being read.
        """
        for subdir, dirs, files in os.walk(location):
            for file in files:
                filepath = subdir + os.sep + file
                filename = os.path.splitext(file)[0]
                f = open(filepath, "r", encoding="utf-8")

                filetext = f.read()
                tokenized_text = set(nltk.word_tokenize(filetext.lower()))
                """
                since the only way to differentiate mails is by name, we check for the filenames starting with 'spmsg'. all those get added to our spam-mail list, the rest to our ham-mail list
                """
                if filename.startswith('spmsg'):
                    spamMails.append(tokenized_text)
                else:
                    hamMails.append(tokenized_text)
                f.close()
                # cant forget to close our files...
        # some debugging output
        print("ham mails: ", len(hamMails))
        print("spam mails: ", len(spamMails))
        # throw it all to our training function
        classifier.train(spamMails, hamMails)

    if args.apply:
        """
        basically the same procedure as with training.
        we move to the correct place in our working directory, and read all mails we find there
        -> our validation data
        """
        spamMails = []
        hamMails = []
        os.chdir('corpus-mails/corpus/validation')
        location = os.getcwd()

        for subdir, dirs, files in os.walk(location):
            for file in files:
                filepath = subdir + os.sep + file

                filename = os.path.splitext(file)[0]
                f = open(filepath, "r", encoding="utf-8")
                filetext = f.read()
                tokenized_text = set(nltk.word_tokenize(filetext.lower()))
                # print(filename)
                if filename.startswith('spmsg'):
                    spamMails.append((tokenized_text, filename))
                else:
                    hamMails.append((tokenized_text, filename))
                f.close()
        print("ham mails: ", len(hamMails))
        print("spam mails: ", len(spamMails))
        classifier.apply(spamMails, hamMails)