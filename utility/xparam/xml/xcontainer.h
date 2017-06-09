#pragma once
#include <memory>
#include <stddef.h>
#include <vector>
#include <assert.h>

/*
XParam 是一个二叉树节点

*/

template <typename T
	, typename KeyType
	, typename FunctorLT
	, typename FunctorLTK
	, typename FunctorEK>
class xcontainer
{
	typedef std::shared_ptr<T> element;
	typedef element const& element_cref;
	typedef element_cref   parent_cref;

	typedef std::vector<element> ElementContainer;
	typedef typename ElementContainer::iterator ElementIter;
	typedef std::unique_ptr<ElementContainer> ElementContainerPtr;

	typedef xcontainer<T
	, KeyType
	, FunctorLT
	, FunctorLTK
	, FunctorEK> this_class;

	struct ElementsCompare
	{
		inline bool operator()
			( element_cref p1
			, element_cref p2) const
		{
			return FunctorLT()(p1, p2);
		}
	};

public:
	xcontainer() 
		: elements_count_(0) 
	{
	}

	~xcontainer()
	{
		clear();
	}

	size_t count() const
	{
		return elements_count_;
	}

	void push_front(element_cref elem, parent_cref pa)
	{
		assert(!elem
			&& !elem->parent()
			&& !elem->prev() 
			&& !elem->next());

		if (!elem || !pa || !elem->parent())
		{
			return;
		}

		elem->prev(nullptr);
		elem->next(head_ptr_);
		if (head_ptr_)
		{
			head_ptr_->prev(elem);
		}
		else
		{
			tail_ptr_ = elem;
		}

		head_ptr_ = elem;
		++elements_count_;
		elem->parent(pa) ;

		if (elements_contaner_)
		{
			ElementIter iter 
				= lower_bound(
				elements_contaner_->begin()
				, elements_contaner_->end()
				, elem
				, ElementsCompare());
			elements_contaner_->insert(iter, elem);
		}
		
	}

	void push_back(element_cref elem, parent_cref pa)
	{
		assert(elem 
			&& pa 
			&& (!elem->parent()) 
			&& (!elem->prev()) 
			&& (!elem->next()));

		if (!elem 
			|| !pa 
			|| elem->parent())
		{
			return;
		}

		elem->prev(tail_ptr_);
		elem->next(nullptr);
		if (tail_ptr_)
		{
			tail_ptr_->next(elem);
		}
		else
		{
			head_ptr_ = elem;
		}

		tail_ptr_ = elem;
		++elements_count_;
		elem->parent(pa);
		if (elements_contaner_)
		{
			ElementIter iter 
				= std::upper_bound(
				elements_contaner_->begin()
				, elements_contaner_->end()
				, elem
				, ElementsCompare());
			elements_contaner_->insert(iter, elem);
		}
	}
	
	void insert_before(element_cref before
		, element_cref elem
		, parent_cref pa)
	{
		if (!before)
		{
			push_front(elem, pa);
			return;
		}

		assert(elem 
			&& pa 
			&& (!elem->parent()) 
			&& (!elem->prev()) 
			&& (!elem->next()));

		if (!elem || !pa || elem->parent())
			return;
		elem->prev(before->prev());
		elem->next(before);
		if (before->prev())
		{
			before->prev()->next(elem);
		}

		before->prev(elem);
		++elements_count_;
		elem->parent(pa);

		if (elements_contaner_)
		{
			if ( ElementsCompare()( before, elem))
			{
				ElementIter iter = std::lower_bound(
					elements_contaner_->begin()
					, elements_contaner_->end()
					, elem
					, ElementsCompare());
				elements_contaner_->insert(iter, elem);
			}
			else if ( ElementsCompare()( elem, before) )
			{
				ElementIter iter = upper_bound(
					elements_contaner_->begin()
					, elements_contaner_->end()
					, elem
					, ElementsCompare());
				elements_contaner_->insert(iter, elem);
			}
			else
			{
				std::pair<ElementIter, ElementIter> result = 
					std::equal_range(elements_contaner_->begin()
					, elements_contaner_->end()
					, before
					, ElementsCompare());

				for (ElementIter iter = result.first
					; iter != result.second
					; ++ iter)
				{
					if (*iter == before)
					{
						elements_contaner_->insert(iter, elem);
						break;
					}
				}
			}
		}
	}

	void insert_after(element_cref after
		, element_cref elem
		, parent_cref pa)
	{
		if (!after)
		{
			push_back(elem, pa);
			return;
		}

		assert(elem 
			&& pa 
			&& (!elem->parent()) 
			&& (!elem->prev()) 
			&& (!elem->next()));

		if (!elem || !pa || elem->parent())
		{
			return;
		}

		elem->next(after->next());
		elem->prev(after);
		if (after->next())
		{
			after->next()->prev(elem);
		}

		after->next(elem);
		++elements_count_;
		elem->parent(pa);
		if (elements_contaner_)
		{
			if ( ElementsCompare()(after, elem))
			{
				ElementIter iter = 
					lower_bound(elements_contaner_->begin()
					, elements_contaner_->end()
					, elem
					, ElementsCompare());
				elements_contaner_->insert(iter, elem);
			}
			else if ( ElementsCompare()(elem, after))
			{
				ElementIter iter 
					= upper_bound(elements_contaner_->begin()
					, elements_contaner_->end()
					, elem
					, ElementsCompare());
				elements_contaner_->insert(iter, elem);
			}
			else
			{
				
				std::pair<ElementIter, ElementIter> result 
					= std::equal_range(elements_contaner_->begin()
					, elements_contaner_->end()
					, after
					, ElementsCompare());
				for (ElementIter iter = result.first
					; iter != result.second
					; ++ iter)
				{
					if (*iter == after)
					{
						elements_contaner_->insert(++iter, elem);
						break;
					}
				}
			}
		}
	}
	
	void remove(element_cref elem, parent_cref pa)
	{
		assert(elem 
			&& pa 
			&& elem->parent() == pa);

		if (!elem 
			|| !pa 
			|| elem->parent() != pa)
		{
			return;
		}

		if (elem->next())
		{
			elem->next()->prev(elem->prev());
		}

		if (elem->prev())
		{
			elem->prev()->next(elem->next());
		}

		if (head_ptr_ == elem)
		{
			head_ptr_ = elem->next();
		}

		if (tail_ptr_ == elem)
		{
			tail_ptr_ = elem->prev();
		}

		elem->next(nullptr);
		elem->prev(nullptr);
		--elements_count_;
		elem->parent(nullptr);

		if (elements_contaner_)
		{
			 
			std::pair<ElementIter, ElementIter> result = 
				std::equal_range(elements_contaner_->begin()
				, elements_contaner_->end()
				, elem
				, ElementsCompare());
			for (ElementIter iter = result.first
				; iter != result.second
				; ++ iter)
			{
				if (*iter == elem)
				{
					elements_contaner_->erase(iter);
					break;
				}
			}
		}
	}
	
	void clear()
	{
		if (!elements_count_)
		{
			return;
		}

		for (element elem = head_ptr_; elem; )
		{
			element tmp = elem->next();
			elem->next(nullptr);
			elem->prev(nullptr);
			elem->parent(nullptr);
			elem = tmp;
		}

		head_ptr_ = nullptr;
		tail_ptr_ = nullptr;
		elements_count_ = 0;

		if (elements_contaner_)
		{
			elements_contaner_->clear();
		}
	}

	element first() const
	{
		return head_ptr_;
	}

	element first(KeyType const& key, size_t position) const
	{
		if (elements_contaner_)
		{
			ElementIter iter 
				= std::lower_bound(elements_contaner_->begin()
				, elements_contaner_->end()
				, key
				, FunctorLTK());

			if (elements_contaner_->end() > iter + position 
				&& FunctorEK()(*(iter + position), key))
			{
				return element(*(iter + position));
			}
			else
			{
				return element();
			}
		}
		else
		{
			size_t counter = 0;
			for (element elem = head_ptr_
				; elem
				; elem = elem->next())
			{
				if (FunctorEK()(elem, key))
				{
					if(counter == position)
					{
						return elem;
					}

					++counter;
				}
			}

			return element();
		}
	}

	element last() const
	{
		return tail_ptr_;
	}

	element last(KeyType const& key, size_t position) const
	{
		if (elements_contaner_)
		{
			ElementIter iter = std::upper_bound
				(elements_contaner_->begin()
				, elements_contaner_->end()
				, key
				, FunctorLTK());
			if (iter - position > elements_contaner_->begin() 
				&& FunctorEK()(*(iter - position - 1), key))
			{
				return *(iter - position - 1);
			}
			else
			{
				return element();
			}
		}
		else
		{
			size_t counter = 0;
			for (element elem = tail_ptr_
				; elem
				; elem = elem->prev())
			{
				if (FunctorEK()(elem, key))
				{
					if(counter == position)
					{
						return elem;
					}

					++counter;
				}
			}
			return element();
		}
	}

	void build_index()
	{
		if (elements_contaner_)
		{
			return ;
		}
		
		elements_contaner_.reset(new ElementContainer());
		if (elements_count_)
		{
			elements_contaner_->reserve(elements_count_);
			for (element elem = head_ptr_
				; elem 
				; elem = elem->next())
			{
				elements_contaner_->push_back(elem);
			}

			std::stable_sort(elements_contaner_->begin()
				, elements_contaner_->end()
				, ElementsCompare());
		}
	}
	
	void clear_index()
	{
		if (elements_contaner_)
		{
			return ;
		}

		elements_contaner_.reset();
	}
	
	bool has_index() const
	{
		return ( nullptr != elements_contaner_ );
	}
private:
	element head_ptr_;
	element tail_ptr_;
	size_t elements_count_;
	ElementContainerPtr elements_contaner_;
};
